#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>

/**
 * Directions are numbered clockwise from the (0, -1) vector.
 */
enum direction {
	DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT, DIR_COUNT
};

/**
 * Two dimensional signed vector.
 *
 * If the vector represents position, the y axis points down and x to the right
 * and is relative to the current cell.
 */
typedef struct vec2_str {
	int8_t x;
	int8_t y;
} vector2;

static inline vector2 mk_vector2(int8_t x, int8_t y)
{
	return (vector2) {x, y};
}

/**
 * Rotate a vector by multiples of 90 degrees in clockwise direction. Negative rotation is supported.
 */
static inline vector2 vector_rotate(vector2 v, int cw) {
	cw %= DIR_COUNT;
	if (cw < 0) {
		cw += DIR_COUNT;
	}
	switch (cw) {
	default:
	case 0:
		return v;
	case 1:
		return mk_vector2(-v.y, v.x);
	case 2:
		return mk_vector2(-v.x, -v.y);
	case 3:
		return mk_vector2(v.y, -v.x);
	}
}

vector2 direction_to_vector(enum direction d);
enum direction reverse_direction(enum direction d);
enum direction rotate_direction(enum direction d, int cw);
enum direction usart_to_direction(uint8_t usart);
uint8_t direction_to_usart(uint8_t usart);

#endif // VECTOR_H
