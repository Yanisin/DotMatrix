#include <stdint.h>
#include <string.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "disp.h"
#include "disp_spi.h"
#include "spi_dma.h"
#include "hw_defs.h"

#define COLOR_DEPTH 32
#define NUM_BLANKS 3
#define DMA_ROW_LEN (COLOR_DEPTH+NUM_BLANKS)

/*
 * pre-computed LUT for gamma 1.8 and
 * 32 values
 */
static const uint8_t gamma18_lut[COLOR_DEPTH] = {
		0, 1, 1, 1, 1, 2, 2, 3, 3, 4,
		5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 17, 19, 20, 22, 23, 25, 26,
		28, 30, 31
};

static uint8_t disp_state[DISP_ROWS_NUM][DISP_COLS_NUM];
static uint16_t disp_row_dmabuf[DISP_ROWS_NUM][DMA_ROW_LEN];

static void disp_out_init(void) 
{
	/* enable clocks for the GPIOs connected to the display */
	rcc_periph_clock_enable(DISP_GPIOS_RCC);

	/*
	 * setup the GPIOs for the Output-Enable pins of the
	 * display drivers
	 */
	gpio_mode_setup(DISP_GPIOP_nOE_ROW,
		GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_GPIO_nOE_ROW);
	gpio_mode_setup(DISP_GPIOP_nOE_COL,
		GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_GPIO_nOE_COL);

	/* enable the drivers */
	gpio_clear(DISP_GPIOP_nOE_ROW, DISP_GPIO_nOE_ROW);
	gpio_clear(DISP_GPIOP_nOE_COL, DISP_GPIO_nOE_COL);
}

static void disp_kick(void)
{
	spi_dma_start((void*)&disp_row_dmabuf[0],
			DMA_ROW_LEN*DISP_ROWS_NUM);
}

void disp_init(void)
{
	/* clean out the display state including DMA row buffers */
	disp_clean();

	/* configure the I/O lines connected to the LED matrix */
	disp_out_init();

	/* configure the SPI channel */
	disp_spi_init();
	/* configure the DMA for the display refresh */
	spi_dma_init();
	/* start the DMA, the display is now on */
	disp_kick();
}

static uint8_t gamma_lookup(uint8_t val)
{
	if (val > 31)
		val = 31;

	return gamma18_lut[val];
}

static void disp_row_update(int row_num)
{
	uint8_t row = 1 << row_num;

	for (int i=0; i < COLOR_DEPTH; i++) {
		uint8_t col = 0;
		for (int c=0; c < DISP_COLS_NUM; c++) {
			if (gamma_lookup(disp_state[row_num][c]) > i)
				col |= 1 << c;
		}
		disp_row_dmabuf[row_num][i] = (col << 8) | row;
	}
}

void disp_update_rows(void)
{
	for (int row = 0; row < DISP_ROWS_NUM; row++)
		disp_row_update(row);
}

void disp_clean(void)
{
	memset(disp_state, 0, sizeof(disp_state));
	memset(disp_row_dmabuf, 0, sizeof(disp_row_dmabuf));
}

void disp_set_noupdate(int col, int row, uint8_t val)
{
	disp_state[row][col] = val;
}
void disp_set(int col, int row, uint8_t val)
{
	disp_set_noupdate(col, row, val);
	disp_row_update(row);
}

uint8_t disp_get(int col, int row)
{
	if (col < 0 || col >= DISP_COLS_NUM)
		return 0;
	if (row < 0 || row >= DISP_ROWS_NUM)
		return 0;

	return disp_state[row][col];
}
