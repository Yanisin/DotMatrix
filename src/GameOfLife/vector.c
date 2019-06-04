#include "vector.h"
#include "hw_defs.h"
#include "console.h"

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

enum direction usart_to_direction(uint8_t usart)
{
	switch (usart) {
	case USART_DIR_UP:
		return DIR_UP;
	case USART_DIR_DOWN:
		return DIR_DOWN;
	case USART_DIR_LEFT:
		return DIR_LEFT;
	case USART_DIR_RIGHT:
		return DIR_RIGHT;
	default:
		return DIR_COUNT;
	}
}

static const uint8_t d2u_array[4] = {
	[DIR_UP] = USART_DIR_UP,
	[DIR_DOWN] = USART_DIR_DOWN,
	[DIR_LEFT] = USART_DIR_LEFT,
	[DIR_RIGHT] = USART_DIR_RIGHT,
};

uint8_t direction_to_usart(uint8_t usart)
{
	return d2u_array[usart];
}
