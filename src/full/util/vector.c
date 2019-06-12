#include "hw_defs.h"
#include <hw/console.h>
#include <util/vector.h>

static const vector2 d2v_array[4] = {
       [DIR_RIGHT] = {1, 0},
       [DIR_DOWN] = {0, 1},
       [DIR_LEFT] = {-1, 0},
       [DIR_UP] = {0, -1},
};

vector2 direction_to_vector(enum direction d)
{
	return d2v_array[d];
}

static const enum direction rdir_array[4] = {
	[DIR_RIGHT] = DIR_LEFT,
	[DIR_LEFT] = DIR_RIGHT,
	[DIR_UP] = DIR_DOWN,
	[DIR_DOWN] = DIR_UP,
};

enum direction reverse_direction(enum direction d)
{
	return rdir_array[d];
}

enum direction rotate_direction(enum direction d, int cw)
{
	cw += d;
	cw %= DIR_COUNT;
	if (cw < 0) {
		cw += DIR_COUNT;
	}
	return (enum direction) cw;
}
