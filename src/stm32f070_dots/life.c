#include <stdint.h>

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

void life_init(void)
{
	life_tick_next = ticker_get_ticks() + LIFE_TICK;
	rand_init();
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
				if (disp_get(i+k, j+l) != 0)
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
		disp_set(col, row, 31);
}

static void life_tick(void)
{
	uint8_t newboard[BOARD_WIDTH][BOARD_HEIGHT];
	int have_life = 0;
	int	i, j, a;

	/* apply B36/S23 rule to all cells */
	for (i=0; i<BOARD_WIDTH; i++) {
		for (j=0; j<BOARD_HEIGHT; j++) {
			/* inspect neighborhood of the cell */
			a = adjacent_to(i, j);

			switch (a) {
			default: /* decay */
				newboard[i][j] = clamp(disp_get(i, j) - 16);
				break;
			case 2: /* stasis */
				newboard[i][j] = disp_get(i, j);
				break;
			case 3: /* growth */
			case 6:
				newboard[i][j] = clamp(disp_get(i, j) + 16);
				break;
			}
		}
	}

	/* copy the new board back into the old board */
	for (i=0; i<BOARD_WIDTH; i++) {
		for (j=0; j<BOARD_HEIGHT; j++) {
			disp_set(i, j, newboard[i][j]);
			have_life += newboard[i][j];
		}
	}

	if (have_life == 0) {
		seeding = 8;
		life_generation = 0;
	}
}

void life_worker(void)
{
	unsigned int tick = ticker_get_ticks();

	if (tick < life_tick_next)
		return;

	life_tick_next = tick + LIFE_TICK;

	while (seeding) {
		add_random_life();
		--seeding;
	}

	life_tick();

	++life_generation;
	if (life_generation > 500) {
		life_generation = 0;
		disp_clean();
	}
}
