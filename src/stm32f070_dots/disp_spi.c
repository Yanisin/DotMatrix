
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>

#include "hw_defs.h"
#include "disp_spi.h"

void disp_spi_init(void)
{
	/* enable clock for the SPI peripheral */
	rcc_periph_clock_enable(DISP_COLS_SPI_RCC);

	/* configure the clock pin */
	gpio_mode_setup(DISP_COLS_CLK_GPIO, GPIO_MODE_AF, GPIO_PUPD_NONE,
			DISP_COLS_CLK_GPIOP);
	gpio_set_af(DISP_COLS_CLK_GPIO, DISP_COLS_CLK_AF_NUM,DISP_COLS_CLK_GPIOP);
	/* configure the data pin */
	gpio_mode_setup(DISP_COLS_DATA_GPIO, GPIO_MODE_AF, GPIO_PUPD_NONE,
			DISP_COLS_DATA_GPIOP);
	gpio_set_af(DISP_COLS_DATA_GPIO, DISP_COLS_DATA_AF_NUM,
			DISP_COLS_DATA_GPIOP);
	/* configure the "strobe" (latch enable) */
	gpio_mode_setup(DISP_COLS_STROBE_GPIOP,
			GPIO_MODE_AF, GPIO_PUPD_NONE,
			DISP_COLS_STROBE_GPIO);
	gpio_set_af(DISP_COLS_STROBE_GPIOP,
			DISP_COLS_STROBE_AF_NUM, DISP_COLS_STROBE_GPIO);

	/* initialize the SPI controller in master mode */
	spi_init_master(DISP_COLS_SPI, SPI_CR1_BAUDRATE_FPCLK_DIV_16, 0,
			0, SPI_CR1_MSBFIRST);

	/* no software slave management, let the hardware handle NSS */
	spi_disable_software_slave_management(DISP_COLS_SPI);
	spi_enable_ss_output(DISP_COLS_SPI);
	/* set data size to 16 bit */
	spi_set_data_size(DISP_COLS_SPI, SPI_CR2_DS_16BIT);
	/* enable NSS pulse mode (no API function for that!) */
	SPI_CR2(DISP_COLS_SPI) |= SPI_CR2_NSSP;
	/* Finally enable the SPI. */
	spi_enable(DISP_COLS_SPI);
}
