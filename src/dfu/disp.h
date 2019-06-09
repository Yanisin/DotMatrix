#pragma once

#include <stdint.h>

#define DISP_ROWS 8

extern uint8_t disp[DISP_ROWS];
void disp_init(void);
void disp_teardown(void);
void disp_show(const uint8_t icon[static DISP_ROWS]);
void disp_tick(void);

extern const uint8_t icon_wait[DISP_ROWS];
extern const uint8_t icon_slave[DISP_ROWS];
extern const uint8_t icon_flash[DISP_ROWS];
extern const uint8_t icon_flash_problem[DISP_ROWS];
