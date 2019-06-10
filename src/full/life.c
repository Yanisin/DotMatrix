#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "hw_defs.h"
#include "applet.h"
//#include "life.h"
#include "disp.h"
#include "rand.h"
#include "usart_buffered.h"
#include "console.h"
#include "led.h"
#include "icons.h"
#include "topo.h"
#include "i2c.h"
#include <ch.h>

#define BOARD_HEIGHT 8
#define BOARD_WIDTH 8

#define LIFE_TICK TIME_MS2I(20)
static sysinterval_t life_tick_interv;
static unsigned int life_tick_mult;
static int seeding;
static int life_generation;
static int scheduled_cleanup;
static int last_life;
static int life_unchanged;

#define HIGHLIFE	1
#define MONOCHROME	0

/**
 * Send edge state from the last generation to the neighbors
 */
#define MSG_TYPE_EDGE MSG_USER_ID(0)
/**
 * Someone wants to update the tick rate.
 */
#define MSG_TYPE_TICK MSG_USER_ID(1)

/**
 * Master synchronizes the tick rate
 */
#define MSG_TYPE_SYNC MSG_USER_ID(3)

#if MONOCHROME
#define LIFE_CELL(i, j) (31)
#else
#define LIFE_CELL(i, j) clamp(board_get(i, j) + 8)
#endif

static uint8_t board[BOARD_WIDTH+2][BOARD_HEIGHT+2];

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
	usart_send_msg(u, MSG_TYPE_EDGE, MSG_DEFAULT, 1, &edge_state);
}

static void transmit_edges(void)
{
	uint8_t edge_state;
	/* We transmit bits on the edges in the clockwise direction */

	/* Upper edge */
	edge_state = 0;
	for (int col = 0; col < BOARD_WIDTH; col++)
		edge_state |= !!board_get(col, 0) << col;
	send_edge_msg(DIR_UP, edge_state);

	/* Right edge */
	edge_state = 0;
	for (int row = 0; row < BOARD_HEIGHT; row++)
		edge_state |= !!board_get(BOARD_WIDTH - 1, row) << row;
	send_edge_msg(DIR_RIGHT, edge_state);

	/* Bottom edge */
	edge_state = 0;
	for (int col = 0; col < BOARD_WIDTH; col++)
		edge_state |= !!board_get(BOARD_WIDTH - 1 - col, BOARD_HEIGHT - 1) << col;
	send_edge_msg(DIR_DOWN, edge_state);

	/* Left edge */
	edge_state = 0;
	for (int row = 0; row < BOARD_HEIGHT; row++)
		edge_state |= !!board_get(0, BOARD_HEIGHT - 1 - row) << row;
	send_edge_msg(DIR_LEFT, edge_state);
}

static void update_edge(enum direction edge, uint8_t edge_state)
{
	/* We have to flip the order of the bits relative to how we have received them.
	 * E.g. Our neighbor has sent us his right edge, so the first bit is row = 0. So we
	 * need to store it to row = 0 too of our left edge (whic makes the bit order counter-clockwise).
	 *
	 * Edge is the direction we have received the data from.
	 */
	// console_printf("edge %d %x\n", edge, edge_state);
	switch (edge) {
	case DIR_UP:
		for (int col = 0; col < BOARD_WIDTH; col++)
			board_set(BOARD_WIDTH - 1 - col, -1, (edge_state & (1<<col)) ? 8 : 0);
		break;
	case DIR_RIGHT:
		for (int row = 0; row < BOARD_HEIGHT; row++)
			board_set(BOARD_WIDTH, BOARD_HEIGHT - 1 - row, (edge_state & (1<<row)) ? 8 : 0);
		break;
	case DIR_DOWN:
		for (int col = 0; col < BOARD_WIDTH; col++)
			board_set(col, BOARD_HEIGHT, (edge_state & (1<<col)) ? 8 : 0);
		break;
	case DIR_LEFT:
		for (int row = 0; row < BOARD_HEIGHT; row++)
			board_set(-1, row, (edge_state & (1<<row)) ? 8 : 0);
		break;
	default:
		break;
	}
}

static void update_life_tick(enum direction from_edge)
{
	enum direction edge;
	uint8_t buf = life_tick_mult;
	console_printf(" %i %02x ", life_tick_mult, buf);

	for (edge = 0; edge < DIR_COUNT; edge++) {
		if (edge != from_edge) {
			usart_send_msg(edge, MSG_TYPE_TICK, MSG_DEFAULT, 1, &buf);
		}
	}
}

static bool receive_msg(void)
{
	msg_header hdr;
	buf_ptr data;
	//console_printf("msg id=%d, len=%d from %d\n", hdr->id, hdr->length, edge);
	if (!msg_rx_queue_get(default_queue, &hdr, &data, TIME_IMMEDIATE))
		return false;

	switch (hdr.id) {
		case MSG_TYPE_EDGE:
			update_edge(hdr.source, *buf_ptr_index(&data, 0));
		break;
		case MSG_TYPE_TICK: {
			unsigned int new_mult = *buf_ptr_index(&data, 0);

			if (new_mult != life_tick_mult) {
				life_tick_mult = new_mult;
				life_tick_interv = life_tick_mult * LIFE_TICK;
				update_life_tick(hdr.source);
			}
		}; break;
	}
	msg_rx_queue_ack(default_queue);
	return true;
}

static int adjacent_to(int i, int j)
{
	int	k, l, count;

	count = 0;

	/* go around the cell */
	for (k = -1; k <= 1; k++) {
		for (l = -1; l <= 1; l++) {
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

static void life_worker(void)
{
	led_toggle();
	/* 2) Wait for next tick */
	if (topo_is_master) {
		chThdSleep(life_tick_interv);
		i2c_broadcast(MSG_TYPE_SYNC, 0, NULL);
	} else {
		msg_header hdr;
		buf_ptr ptr;
		if(msg_rx_queue_get(alt_queue, &hdr, &ptr, TIME_S2I(1)))
			msg_rx_queue_ack(alt_queue);
	}
	/* wait a bit to make sure everybody got it */
	chThdSleep(TIME_MS2I(2));


	/* 2) Check everything that might have happened (messages, console) */
	while (receive_msg())
		;
	int c = console_getc();
	if (c == '+' || c == '-') {
		if (c == '+' && life_tick_mult < 0x3f)
			++life_tick_mult;
		if (c == '-' && life_tick_mult > 0)
			--life_tick_mult;

		life_tick_interv = life_tick_mult * LIFE_TICK;
		update_life_tick(-1);
	}

	/* 4) Advance the life */
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

	/* 3) Send updates */
	transmit_edges();

}

static void life_run(void)
{
	seeding = 12;
	life_tick_mult = 8;
	life_tick_interv = life_tick_mult * LIFE_TICK;

	memset(board, 0, sizeof(board));
	rand_init();

	while(!applet_should_end) {
		life_worker();
	}
}

static msg_rx_queue* life_msg(const msg_header *msg)
{
	if(msg->id == MSG_TYPE_SYNC)
		return alt_queue;
	else
		return default_queue;
}

static const struct applet life_applet = {
	.run = life_run,
	.dispatch_msg = life_msg,
	.priority = 100,
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
