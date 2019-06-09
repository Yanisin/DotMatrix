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
static int disp_rotation;
thread_t *display_test;

static inline vector2 rot(int col, int row)
{
	/* Y axis on vector2 points up, whereas display vectors go down, which gives them diferent basis */
	return disp_rotate_coords(mk_vector2(col, row), DISP_COLS_NUM, disp_rotation);
}

void disp_set_rotation(int cw)
{
	disp_rotation = cw;
}

static void disp_out_init(void) 
{
	/* enable clocks for the GPIOs connected to the display */
	rcc_periph_clock_enable(DISP_GPIOS_RCC);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);

	/*
	 * setup the GPIOs for the Output-Enable pins of the
	 * display drivers
	 */
	gpio_mode_setup(DISP_nOE_ROW_GPIO,
		GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_nOE_ROW_GPIO_PIN);
	gpio_mode_setup(DISP_nOE_COL_GPIO,
		GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_nOE_COL_GPIO_PIN);

	/* enable the drivers */
	gpio_clear(DISP_nOE_ROW_GPIO, DISP_nOE_ROW_GPIO_PIN);
	gpio_clear(DISP_nOE_COL_GPIO, DISP_nOE_COL_GPIO_PIN);
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
#if 0
static uint8_t fix_ver_04(uint8_t in)
{
	uint8_t out;

	out = in & 0xf;
	out |= (in & 0x10) << 3;
	out |= (in & 0x20) << 1;
	out |= (in & 0x40) >> 1;
	out |= (in & 0x80) >> 3;
	return out;

}
#endif

static uint8_t fix_ver_05(uint8_t in)
{
	uint8_t out;

	out = (in & 0x1) << 7;
	out |= (in & 0x2) << 5;
	out |= (in & 0x4) << 3;
	out |= (in & 0x8) << 1;
	out |= (in & 0x10) >> 1;
	out |= (in & 0x20) >> 3;
	out |= (in & 0x40) >> 5;
	out |= (in & 0x80) >> 7;
	return out;

}


static void disp_row_update(int row_num)
{
	uint8_t col_fixed,row_fixed;
	uint8_t row = 0x80 >> row_num;

	for (int i=0; i < COLOR_DEPTH; i++) {
		uint8_t col = 0;
		for (int c=0; c < DISP_COLS_NUM; c++) {
			if (gamma_lookup(disp_state[row_num][c]) > i)
				col |= 0x80 >> c;
		}
//		col = fix_ver_04(col);
		col_fixed = fix_ver_05(col);
		row_fixed = fix_ver_05(row);

		disp_row_dmabuf[row_num][i] = (col_fixed << 8) | row_fixed;
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
	vector2 pos = rot(col, row);
	disp_state[pos.y][pos.x] = val;
}
void disp_set(int col, int row, uint8_t val)
{
	vector2 pos = rot(col, row);
	disp_state[pos.y][pos.x] = val;
	disp_row_update(pos.y);
}

uint8_t disp_get(int col, int row)
{
	vector2 pos = rot(col, row);
	if (pos.x < 0 || pos.x >= DISP_COLS_NUM)
		return 0;
	if (pos.y < 0 || pos.y >= DISP_ROWS_NUM)
		return 0;

	return disp_state[pos.y][pos.x];
}
