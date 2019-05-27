#include "../disp.h"
#include <memory.h>
#include "sim_proto.h"
#include "proto_defs.h"

#define DISP_ROWS_NUM 8
#define DISP_COLS_NUM 8

static uint8_t disp_state[DISP_ROWS_NUM][DISP_COLS_NUM];

static void update_row(int row) {
	char buf[DISP_COLS_NUM + 1];
	buf[0] = row;
	memcpy(buf + 1, disp_state[row], DISP_COLS_NUM);
	sim_send(MSGID_UPDATE_DISP, 1 + DISP_COLS_NUM, buf);
}

void disp_init(void)
{
}

void disp_clean(void)
{
	memset(disp_state, 0, sizeof(disp_state));
}

void disp_set(int col, int row, uint8_t val)
{
	disp_state[row][col] = val;
	update_row(row);
}

extern void disp_set_noupdate(int col, int row, uint8_t val)
{
	disp_state[row][col] = val;
}

extern void disp_update_rows(void)
{
	for (int i = 0; i < DISP_ROWS_NUM; i++) {
		update_row(i);
	}
}
extern uint8_t disp_get(int col, int row)
{
	return disp_state[row][col];
}
