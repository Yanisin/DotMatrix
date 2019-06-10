#include <libopencm3/stm32/gpio.h>
#include "disp.h"
#include "hw_defs.h"
#include <string.h>

static uint8_t current_row;

void disp_init(void)
{
	gpio_mode_setup(DISP_nOE_ROW_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_nOE_ROW_GPIO_PIN);
	gpio_mode_setup(DISP_nOE_COL_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_nOE_COL_GPIO_PIN  );
	gpio_mode_setup(DISP_COLS_CLK_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_COLS_CLK_GPIO_PIN  );
	gpio_mode_setup(DISP_COLS_DATA_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_COLS_DATA_GPIO_PIN  );
	gpio_mode_setup(DISP_COLS_STROBE_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_COLS_STROBE_GPIO_PIN  );
	gpio_clear(DISP_nOE_ROW_GPIO, DISP_nOE_ROW_GPIO_PIN);
	gpio_clear(DISP_nOE_COL_GPIO, DISP_nOE_COL_GPIO_PIN);
}

void disp_show(const uint8_t icon[static DISP_ROWS])
{
	memcpy(disp, icon, DISP_ROWS);
}

void disp_teardown(void)
{

}

void disp_tick(void)
{
	int k;
	uint8_t row = disp[current_row];
	for (k = 0;k < 8; k++) {
		if(row & (1 << k)) {
			gpio_set(DISP_COLS_DATA_GPIO, DISP_COLS_DATA_GPIO_PIN);
		} else {
			gpio_clear(DISP_COLS_DATA_GPIO, DISP_COLS_DATA_GPIO_PIN);
		}
		__asm__("nop");
		gpio_set(DISP_COLS_CLK_GPIO, DISP_COLS_CLK_GPIO_PIN);
		__asm__("nop");
		gpio_clear(DISP_COLS_CLK_GPIO, DISP_COLS_CLK_GPIO_PIN);
		__asm__("nop");
	}

	for (k = 0; k < 8;k++) {
		if(current_row == k) {
			gpio_set(DISP_COLS_DATA_GPIO, DISP_COLS_DATA_GPIO_PIN);
		} else {
			gpio_clear(DISP_COLS_DATA_GPIO, DISP_COLS_DATA_GPIO_PIN);
		}
		__asm__("nop");
		gpio_set(DISP_COLS_CLK_GPIO, DISP_COLS_CLK_GPIO_PIN);
		__asm__("nop");
		gpio_clear(DISP_COLS_CLK_GPIO, DISP_COLS_CLK_GPIO_PIN);
		__asm__("nop");
	}


	gpio_set(DISP_COLS_STROBE_GPIO, DISP_COLS_STROBE_GPIO_PIN);
	__asm__("nop");
	gpio_clear(DISP_COLS_STROBE_GPIO, DISP_COLS_STROBE_GPIO_PIN);
	__asm__("nop");
	current_row = (current_row + 1) % DISP_ROWS;

}
uint8_t disp[DISP_ROWS];


#define _ 0
#define X 1

#define ICON_ROW(a, b, c, d, e, f, g, h) \
	(((a) << 0) | ((b) << 1) | ((c) << 2) | ((d) << 3) \
	| ((e) << 4) | ((f) << 5) | ((g) << 6) | ((h) << 7))

const uint8_t icon_wait[8] = {
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, X, _, X, _, X, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
};

const uint8_t icon_slave[8] = {
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, X, X, X, _, _, _),
	ICON_ROW(_, _, X, _, _, _, _, _),
	ICON_ROW(_, _, X, X, X, _, _, _),
	ICON_ROW(_, _, _, _, X, _, _, _),
	ICON_ROW(_, _, X, X, X, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
};

const uint8_t icon_flash[8] = {
	ICON_ROW(_, _, X, X, X, X, _, _),
	ICON_ROW(_, X, X, _, _, X, X, _),
	ICON_ROW(_, _, X, _, _, X, _, _),
	ICON_ROW(_, X, X, _, _, X, X, _),
	ICON_ROW(_, _, X, _, _, X, _, _),
	ICON_ROW(_, X, X, _, _, X, X, _),
	ICON_ROW(_, _, X, X, X, X, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
};

const uint8_t icon_flash_problem[8] = {
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(_, _, X, _, _, X, _, _),
	ICON_ROW(_, _, _, X, X, _, _, _),
	ICON_ROW(_, _, _, X, X, _, _, _),
	ICON_ROW(_, _, X, _, _, X, _, _),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
};

