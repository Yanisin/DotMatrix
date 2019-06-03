#include "../disp.h"
#include <memory.h>
#include "sim_proto.h"
#include "proto_defs.h"
#include "../vector.h"
#include "../console.h"

#define DISP_ROWS_NUM 8
#define DISP_COLS_NUM 8

static uint8_t disp_state[DISP_ROWS_NUM][DISP_COLS_NUM];
static int disp_rotation;

static inline vector2 rot(int col, int row)
{
	/* Y axis on vector2 points up, whereas display vectors go down, which gives them diferent basis */
	return disp_rotate_coords(mk_vector2(col, row), DISP_COLS_NUM, disp_rotation);
}

static void update_row(int row) {
	char buf[DISP_COLS_NUM + 1];
	buf[0] = row;
	memcpy(buf + 1, disp_state[row], DISP_COLS_NUM);
	sim_send(MSGID_UPDATE_DISP, 1 + DISP_COLS_NUM, buf);
}

void disp_set_rotation(int cw)
{
	disp_rotation = cw;
}

void disp_init(void)
{
}

void disp_clean(void)
{
	memset(disp_state, 0, sizeof(disp_state));
	disp_update_rows();
}

void disp_set(int col, int row, uint8_t val)
{
	vector2 pos = rot(col, row);
	disp_state[pos.y][pos.x] = val;
	update_row(pos.y);
}

extern void disp_set_noupdate(int col, int row, uint8_t val)
{
	vector2 pos = rot(col, row);
	disp_state[pos.y][pos.x] = val;
}

extern void disp_update_rows(void)
{
	for (int i = 0; i < DISP_ROWS_NUM; i++) {
		update_row(i);
	}
}
extern uint8_t disp_get(int col, int row)
{
	vector2 pos = rot(col, row);
	return disp_state[pos.y][pos.x];
}
