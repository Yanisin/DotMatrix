#include <stdint.h>
#include <string.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "hw_defs.h"
#include "life.h"
#include "disp.h"
#include "ticker.h"
#include "rand.h"
#include "usart_buffered.h"

#define BOARD_HEIGHT 8
#define BOARD_WIDTH 8

#define LIFE_TICK 150
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
static int usart[NUM_EDGES] = {
		USART_DIR_UP, USART_DIR_RIGHT, USART_DIR_DOWN, USART_DIR_LEFT
};

void life_init(void)
{
	memset(board, 0, sizeof(board));
	rand_init();
	seeding = 12;

	gpio_mode_setup(GPIOB,
		GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO8|GPIO9);
	gpio_set_output_options(GPIOB,
			GPIO_OTYPE_OD, GPIO_OSPEED_LOW, GPIO8|GPIO9);
	gpio_clear(GPIOB, GPIO8|GPIO9);

	worker_state = WAIT_TICK;
	life_tick_next = ticker_get_ticks() + LIFE_TICK;
	gpio_clear(GPIOF, GPIO0);
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
		send_char(usart[edge], edge_state);

	}
}

static void receive_edges(void)
{
	enum board_edge edge;
	uint32_t edge_state;

	for (edge = TOP_EDGE; edge < NUM_EDGES; edge++) {
		edge_state = get_char(usart[edge]);
		if (edge_state == (uint32_t)-1)
			edge_state = 0;

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
	int row = rand_get() % 8;
	int col = rand_get() % 8;
	uint8_t val = board_get(col, row);

	if (val == 0)
		board_set(col, row, 1);
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
				if (!board_get(i, j)) {
					newboard[i][j] = LIFE_CELL(i, j);
				}
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

	for (i=0; i<BOARD_WIDTH; i++) {
		for (j=0; j<BOARD_HEIGHT; j++) {
			disp_set(i, j, board_get(i, j));
		}
	}

	if (have_life == last_life) {
		++life_unchanged;
	} else {
		life_unchanged = 0;
		last_life = have_life;
	}

	if (life_unchanged > 50) {
		if (!scheduled_cleanup)
			scheduled_cleanup = 1;
	}

	++life_generation;

}

void life_worker(void)
{
	unsigned int tick = ticker_get_ticks();

	if (worker_state == WAIT_TICK) {
		if ((int)(tick - life_tick_next) < 0)
			return;

		worker_state = WAIT_ENTRY;
	}

	if (worker_state == WAIT_ENTRY) {
		/* release the idle signal */
		gpio_set(GPIOB, GPIO8|GPIO9);
		/* if somebody else holds it down, wait */
		while (gpio_get(GPIOB, GPIO8|GPIO9) == 0)
			;

		worker_state = WORKER_BUSY;

		life_tick_next = ticker_get_ticks() + LIFE_TICK;

/* set idle signal again */
		gpio_clear(GPIOB, GPIO8|GPIO9);

		/* toggle the LED */
		gpio_toggle(GPIOF, GPIO0);
	}

	if (worker_state == WORKER_BUSY) {

		receive_edges();

		/* seed life */
		while (seeding) {
			add_random_life();
			--seeding;
		}

		/* run the beat of life */
		life_tick();

		/* stasis or extinction detected */
		if (scheduled_cleanup) {
			if (scheduled_cleanup == 1) {
				disp_clean();
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
