
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>

#include <stdint.h>

#include "hw_defs.h"
#include "disp_spi.h"

static void show(void);

static void my_spi_init(void)
{

	rcc_periph_clock_enable(DISP_COLS_SPI_RCC);

	gpio_mode_setup(DISP_COLS_CLK_GPIO, GPIO_MODE_AF, GPIO_PUPD_NONE,
			DISP_COLS_CLK_GPIOP);
	gpio_set_af(DISP_COLS_CLK_GPIO, DISP_COLS_CLK_AF_NUM,DISP_COLS_CLK_GPIOP);

	gpio_mode_setup(DISP_COLS_DATA_GPIO, GPIO_MODE_AF, GPIO_PUPD_NONE,
			DISP_COLS_DATA_GPIOP);
	gpio_set_af(DISP_COLS_DATA_GPIO, DISP_COLS_DATA_AF_NUM,DISP_COLS_DATA_GPIOP);

	spi_init_master(DISP_COLS_SPI, SPI_CR1_BAUDRATE_FPCLK_DIV_4, 0,
			SPI_CR1_CPHA, SPI_CR1_MSBFIRST);
	spi_enable_ss_output(DISP_COLS_SPI);
	/* Finally enable the SPI. */
	spi_enable(DISP_COLS_SPI);
}


static void stcp_init(void)
{
	gpio_mode_setup(DISP_COLS_STROBE_GPIO, 
			GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
			DISP_COLS_STROBE_GPIOP);

	gpio_clear(DISP_COLS_STROBE_GPIO, DISP_COLS_STROBE_GPIOP);
	show();
}

static void show(void) 
{
	gpio_set(DISP_COLS_STROBE_GPIO, DISP_COLS_STROBE_GPIOP);
	__asm__("nop");
	__asm__("nop");
	gpio_clear(DISP_COLS_STROBE_GPIO, DISP_COLS_STROBE_GPIOP);
}



void disp_spi_col_set(uint8_t data)
{

	//bitbang_send(data);
	spi_send(SPI1, data);
	/* wait for sending */
	while (SPI_SR(SPI1) & SPI_SR_BSY && !(SPI_SR(SPI1) & SPI_SR_TXE));

	/* FIXME make this with DMA and interrupt? */
	show();
}

void disp_spi_init(void)
{

	rcc_periph_clock_enable(DISP_COLS_GPIO_RCC);
	my_spi_init();
	//bitbang_init();
	stcp_init();
}
