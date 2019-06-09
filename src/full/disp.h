#ifndef DISP_H
#define DISP_H

#include "stdint.h"
#include "vector.h"
#include "ch.h"

extern thread_t *display_test;

extern void disp_init(void);
extern void disp_clean(void);
extern void disp_set_rotation(int cw);
extern void disp_set(int col, int row, uint8_t val);
extern void disp_set_noupdate(int col, int row, uint8_t val);
extern void disp_update_rows(void);
extern uint8_t disp_get(int col, int row);

static inline vector2 disp_rotate_coords(vector2 v, uint8_t size, int cw) {
	/* We indeed can not use the vector rotation functions. We are rotating pixel coordinates
	 * (edges of pixels) and the axis are different. */
	cw %= 4;
	if (cw < 0) {
		cw += 4;
	}
	size -= 1;

	switch (cw) {
	default:
	case 0:
		return v;
	case 1:
		return mk_vector2(size - v.y, v.x);
	case 2:
		return mk_vector2(size - v.x, size - v.y);
	case 3:
		return mk_vector2(v.y, size - v.x);
	}
}

#endif

