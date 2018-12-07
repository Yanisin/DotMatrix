#include <stdint.h>
#include <string.h>

#include "life.h"
#include "disp.h"
#include "ticker.h"
#include "rand.h"

#define BOARD_HEIGHT 8
#define BOARD_WIDTH 8

#define LIFE_TICK 200
static unsigned int life_tick_next;
static int seeding;
static int life_generation;
static int scheduled_cleanup;
static int last_life;
static int life_unchanged;

#define HIGHLIFE	1
#define MONOCHROME	0

#if MONOCHROME
#define LIFE_CELL(i, j) (31)
#else
#define LIFE_CELL(i, j) clamp(disp_get(i, j) + 8)
#endif

void life_init(void)
{
	life_tick_next = ticker_get_ticks() + LIFE_TICK;
	rand_init();
	seeding = 8;
}

static int board_get(int col, int row)
{
	col %= BOARD_WIDTH;
	row %= BOARD_HEIGHT;

	return disp_get(col, row);
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
	uint8_t val = disp_get(col, row);

	if (val == 0)
		disp_set(col, row, 1);
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
				if (disp_get(i, j))
					newboard[i][j] = LIFE_CELL(i, j);
				break;
			case 3: /* birth or survival */
					newboard[i][j] = LIFE_CELL(i, j);
					break;
#if HIGHLIFE
			case 6:
				if (!disp_get(i, j)) {
					newboard[i][j] = LIFE_CELL(i, j);
					break;
				}
				/* fall-through */
#endif
			default: /* death */
				break;
			}
		}
	}

	/* copy the new board back into the old board */
	for (i=0; i<BOARD_WIDTH; i++) {
		for (j=0; j<BOARD_HEIGHT; j++) {
			if (disp_get(i, j) != newboard[i][j]) {
				disp_set(i, j, newboard[i][j]);
			}
			have_life += newboard[i][j];
		}
	}

	if (have_life == last_life) {
		++life_unchanged;
	} else {
		life_unchanged = 0;
		last_life = have_life;
	}

	if (life_unchanged > 10) {
		if (!scheduled_cleanup)
			scheduled_cleanup = 1;
	}

	++life_generation;
}

void life_worker(void)
{
	unsigned int tick = ticker_get_ticks();

	if (tick < life_tick_next)
		return;

	life_tick_next = tick + LIFE_TICK;

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
			seeding = 16;
			life_generation = 0;
			last_life = 0;
			life_unchanged = 0;
		}
		--scheduled_cleanup;
	}

	/* restart after 1000 generations */
	if (life_generation > 1000) {
		life_generation = 0;
		disp_clean();
	}
}
