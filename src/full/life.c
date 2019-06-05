#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "hw_defs.h"
#include "applet.h"
//#include "life.h"
#include "disp.h"
#include "ticker.h"
#include "rand.h"
#include "usart_buffered.h"
#include "console.h"
#include "led.h"
#include "common_gpio.h"
#include "icons.h"

#define BOARD_HEIGHT 8
#define BOARD_WIDTH 8

#define LIFE_TICK 20
static int life_tick_interv;
static unsigned int life_tick_mult;
static unsigned int life_tick_next;
static int seeding;
static int life_generation;
static int scheduled_cleanup;
static int last_life;
static int life_unchanged;

enum worker_state {
	WAIT_TICK,
	WAIT_ENTRY,
	WORKER_BUSY,
};
static enum worker_state worker_state;

#define HIGHLIFE	1
#define MONOCHROME	0
#define MSG_TYPE_EDGE MSG_USER_ID(1)
#define MSG_TYPE_TICK MSG_USER_ID(2)
#define MSG_TYPE_INIT MSG_USER_ID(3)

#if MONOCHROME
#define LIFE_CELL(i, j) (31)
#else
#define LIFE_CELL(i, j) clamp(board_get(i, j) + 8)
#endif

static uint8_t board[BOARD_WIDTH+2][BOARD_HEIGHT+2];

enum board_edge {
	TOP_EDGE = 0,
	RIGHT_EDGE,
	BOTTOM_EDGE,
	LEFT_EDGE,
	NUM_EDGES
};

static struct {
	int num;
	int invert;
	bool initialized;
} usart[NUM_EDGES] = {
		{ USART_DIR_UP, 0, false },
		{ USART_DIR_RIGHT, 0, false },
		{ USART_DIR_DOWN, 0, false },
		{ USART_DIR_LEFT, 0, false }
};

static const int edge_invert[NUM_EDGES][NUM_EDGES] = {
		{ 1, 1, 0, 0 },
		{ 1, 1, 0, 0 },
		{ 0, 0, 1, 1 },
		{ 0, 0, 1, 1 },
};

/**
 * bitwise reverse
 */
static uint8_t revers8bit(uint8_t x)
{
	unsigned char b = x;
	b = ((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
	return b;
}

static int board_get(int col, int row)
{
	return board[col+1][row+1];
}

static void board_set(int col, int row, int val)
{
	board[col+1][row+1] = val;
}

static void board_clean(void)
{
	for (int row = 0; row < BOARD_HEIGHT; row++) {
		for (int col = 0; col < BOARD_WIDTH; col++) {
			board_set(col, row, 0);
		}
	}
	console_putc('C');
}

static void send_edge_msg(int u, uint8_t edge_state)
{
	usart_send_msg(u, MSG_TYPE_EDGE, 1, &edge_state);
}

static void transmit_edges(void)
{
	enum board_edge edge;
	uint8_t edge_state;

	for (edge = TOP_EDGE; edge < NUM_EDGES; edge++) {
		edge_state = 0;

		switch (edge) {
		case TOP_EDGE:
			for (int col = 0; col < BOARD_WIDTH; col++)
				edge_state |= !!board_get(col, 0) << col;
			break;
		case BOTTOM_EDGE:
			for (int col = 0; col < BOARD_WIDTH; col++)
				edge_state |= !!board_get(col, BOARD_HEIGHT-1) << col;
			break;
		case LEFT_EDGE:
			for (int row = 0; row < BOARD_HEIGHT; row++)
				edge_state |= !!board_get(0, row) << row;
			break;
		case RIGHT_EDGE:
			for (int row = 0; row < BOARD_HEIGHT; row++)
				edge_state |= !!board_get(BOARD_WIDTH-1, row) << row;
			break;
		default:
			break;
		}
		send_edge_msg(usart[edge].num, edge_state);
	}
}

static void update_edge(enum board_edge edge, uint8_t edge_state, int invert)
{
	if (invert)
		edge_state = revers8bit(edge_state);

	// console_printf("edge %d %x\n", edge, edge_state);
	switch (edge) {
	case TOP_EDGE:
		for (int col = 0; col < BOARD_WIDTH; col++)
			board_set(col, -1, (edge_state & (1<<col)) ? 8 : 0);
		break;
	case BOTTOM_EDGE:
		for (int col = 0; col < BOARD_WIDTH; col++)
			board_set(col, BOARD_HEIGHT, (edge_state & (1<<col)) ? 8 : 0);
		break;
	case LEFT_EDGE:
		for (int row = 0; row < BOARD_HEIGHT; row++)
			board_set(-1, row, (edge_state & (1<<row)) ? 8 : 0);
		break;
	case RIGHT_EDGE:
		for (int row = 0; row < BOARD_HEIGHT; row++)
			board_set(BOARD_WIDTH, row, (edge_state & (1<<row)) ? 8 : 0);
		break;
	default:
		break;
	}
}

static void update_life_tick(enum board_edge from_edge)
{
	enum board_edge edge;
	uint8_t buf = life_tick_mult;
	console_printf(" %i %02x ", life_tick_mult, buf);

	for (edge = TOP_EDGE; edge < NUM_EDGES; edge++) {
		if (edge != from_edge) {
			usart_send_msg(usart[edge].num, MSG_TYPE_TICK, 1, &buf);
		}
	}
}

static void init_edge(enum board_edge edge, char val)
{

	enum board_edge remote_edge = NUM_EDGES;

	switch (val) {
	case 'T':
		remote_edge = TOP_EDGE;
		break;
	case 'B':
		remote_edge = BOTTOM_EDGE;
		break;
	case 'L':
		remote_edge = LEFT_EDGE;
		break;
	case 'R':
		remote_edge = RIGHT_EDGE;
		break;
	default:
		remote_edge = NUM_EDGES;
		break;
	}
	if (remote_edge != NUM_EDGES) {
		usart[edge].initialized = true;
		usart[edge].invert = edge_invert[edge][remote_edge];
	}
}

static bool receive_msg(const usart_header *hdr)
{
	enum board_edge edge = NUM_EDGES;

	for (int i = 0; i < NUM_EDGES; i++) {
		if (usart[i].num == hdr->usart)
			edge = i;
	}
	if (edge == NUM_EDGES)
		return false;

	//console_printf("msg id=%d, len=%d from %d\n", hdr->id, hdr->length, edge);

	switch (hdr->id) {
	case MSG_TYPE_EDGE: {
		uint8_t edge_state;
		if(usart_recv_msg(hdr->usart, 1, &edge_state)) {
			update_edge(edge, edge_state, usart[edge].invert);
		}
		return true;
	}; break;

	case MSG_TYPE_INIT: {
		char remote_edge;
		if(usart_recv_msg(hdr->usart, 1, &remote_edge)) {
			init_edge(edge, remote_edge);
		}
		return true;
	}; break;

	case MSG_TYPE_TICK: {
		uint8_t buf;
		if(usart_recv_msg(hdr->usart, 1, &buf)) {
			unsigned int new_mult = buf;

			if (new_mult != life_tick_mult) {
				life_tick_mult = new_mult;
				life_tick_interv = life_tick_mult * LIFE_TICK;
				update_life_tick(edge);
			}
		}
		return true;
	}; break;

	default:
		return false;
	}
}

static int adjacent_to(int i, int j)
{
	int	k, l, count;

	count = 0;

	/* go around the cell */
	for (k=-1; k<=1; k++) {
		for (l=-1; l<=1; l++) {
			/* only count if at least one of k,l isn't zero */
			if (k || l) {
				if (board_get(i+k, j+l) != 0)
					count++;
			}
		}
	}

	return count;
}

static uint8_t clamp(int val)
{
	if (val < 0)
		return 0;
	if (val > 31)
		return 31;
	return val;
}

static void add_random_life(void)
{
	int row = rand_get() % BOARD_HEIGHT;
	int col = rand_get() % BOARD_WIDTH;
	uint8_t val = board_get(col, row);

	if (val == 0)
		board_set(col, row, 1);
}

static void show_board(void)
{
	for (int i = 0; i < BOARD_WIDTH; i++) {
		for (int j = 0; j < BOARD_HEIGHT; j++) {
			disp_set_noupdate(i, j, board_get(i, j));
		}
	}
	disp_update_rows();
}

static void life_tick(void)
{
	uint8_t newboard[BOARD_WIDTH][BOARD_HEIGHT];
	int have_life = 0;
	int	i, j, a;

	memset(newboard, 0, sizeof(newboard));

	/* apply B36/S23 rule to all cells */
	for (i=0; i<BOARD_WIDTH; i++) {
		for (j=0; j<BOARD_HEIGHT; j++) {
			/* inspect neighborhood of the cell */
			a = adjacent_to(i, j);

			switch (a) {
			case 2: /* survival */
				if (board_get(i, j))
					newboard[i][j] = LIFE_CELL(i, j);
				break;
			case 3: /* birth or survival */
					newboard[i][j] = LIFE_CELL(i, j);
					break;
#if HIGHLIFE
			case 6:
				newboard[i][j] = LIFE_CELL(i, j);
				break;
#endif
			default: /* death */
				break;
			}
		}
	}

	/* copy the new board back into the old board */
	for (i=0; i<BOARD_WIDTH; i++) {
		for (j=0; j<BOARD_HEIGHT; j++) {
			if (board_get(i, j) != newboard[i][j]) {
				board_set(i, j, newboard[i][j]);
			}
			have_life += newboard[i][j];
		}
	}

	show_board();

	if (have_life == last_life) {
		++life_unchanged;
	} else {
		life_unchanged = 0;
		last_life = have_life;
	}

	if (!have_life || life_unchanged > 50) {
		if (!scheduled_cleanup)
			scheduled_cleanup = 1;
	}

	++life_generation;

}

static void life_start(void)
{
	life_tick_mult = 8;
	life_tick_interv = life_tick_mult * LIFE_TICK;
	life_tick_next = ticker_get_ticks() + life_tick_interv;
	worker_state = WAIT_TICK;
}

static void life_worker(void)
{
	unsigned int tick = ticker_get_ticks();
	uint32_t c;

	if (worker_state == WAIT_TICK) {
		if ((int)(tick - life_tick_next) < 0)
			return;

		worker_state = WAIT_ENTRY;
	}

	if (worker_state == WAIT_ENTRY) {
		/* release the idle signal */
		common_gpio_set(true);

		/* if somebody else holds it down, wait */
		while (common_gpio_get() == false)
			;
		/* wait a bit to make sure everybody got it */
		ticker_msleep(2);
		/* set idle signal again */
		common_gpio_set(false);

		//console_puts("T\n");

		/* toggle the LED */
		led_toggle();

		worker_state = WORKER_BUSY;

		life_tick_next = ticker_get_ticks() + life_tick_interv;
	}

	if (worker_state == WORKER_BUSY) {

		c = console_getc();
		if (c == '+' || c == '-') {
			if (c == '+' && life_tick_mult < 0x3f)
				++life_tick_mult;
			if (c == '-' && life_tick_mult > 0)
				--life_tick_mult;

			life_tick_interv = life_tick_mult * LIFE_TICK;
			update_life_tick(-1);
		}

		/* seed life */
		while (seeding) {
			console_putc('S');
			add_random_life();
			--seeding;
		}

		/* run the beat of life */
		life_tick();

		/* stasis or extinction detected */
		if (scheduled_cleanup) {
			if (scheduled_cleanup == 1) {
				board_clean();
				seeding = 12;
				life_generation = 0;
				last_life = 0;
				life_unchanged = 0;
			}
			--scheduled_cleanup;
		}

		/* restart after 1000 generations */
		if (life_generation > 1000) {
			life_generation = 0;
			board_clean();
		}

		transmit_edges();

		worker_state = WAIT_TICK;
	}
}

static void life_init(void)
{
	uint8_t b;
	memset(board, 0, sizeof(board));
	rand_init();
	seeding = 12;
	common_gpio_set(false);
	console_printf("Life starting\n");

	b = 'T';
	usart_send_msg(usart[TOP_EDGE].num, MSG_TYPE_INIT, 1, &b);

	b = 'B';
	usart_send_msg(usart[BOTTOM_EDGE].num, MSG_TYPE_INIT, 1, &b);

	b = 'L';
	usart_send_msg(usart[LEFT_EDGE].num, MSG_TYPE_INIT, 1, &b);

	b = 'R';
	usart_send_msg(usart[RIGHT_EDGE].num, MSG_TYPE_INIT, 1, &b);

	life_start();
}

static const struct applet life_applet = {
	.init = life_init,
	.worker = life_worker,
	.check_usart = receive_msg,
	.icon = {
		ICON_ROW(0, 0, 0, 0, 0, 1, 0, 0),
		ICON_ROW(0, 1, 0, 1, 0, 0, 1, 0),
		ICON_ROW(0, 1, 0, 1, 0, 0, 1, 0),
		ICON_ROW(0, 1, 0, 0, 1, 0, 0, 0),
		ICON_ROW(0, 1, 0, 0, 0, 0, 0, 0),
		ICON_ROW(0, 1, 0, 0, 0, 0, 0, 0),
		ICON_ROW(0, 1, 1, 1, 1, 0, 0, 0),
		ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	}
};

applet_add(life);
