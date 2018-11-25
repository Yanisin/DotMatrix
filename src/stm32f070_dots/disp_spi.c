#include "stdint.h"
#include "disp_spi.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>




static void my_spi_init(void)
{

	rcc_periph_clock_enable(RCC_SPI1);

	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE,
			GPIO3 | GPIO4 |  GPIO5);
	gpio_set_af(GPIOB, GPIO_AF0, GPIO3 |GPIO4 | GPIO5);

	/* Setup SPI parameters. */
//	spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_4, SPI_CR1_CPOL,
			//SPI_CR1_CPHA, SPI_CR1_MSBFIRST);
	/* FIXME read from datasheed if rising all falling edge ofclock is better
	 */
	spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_4, 0,
			SPI_CR1_CPHA, SPI_CR1_MSBFIRST);
	spi_enable_ss_output(SPI1); /* Required, see NSS, 25.3.1 section. */

	/* Finally enable the SPI. */
	spi_enable(SPI1);

}



static void show(void);
static void stcp_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	gpio_mode_setup(GPIOA, 
		GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);
	
			gpio_clear(GPIOA, GPIO15);
			show();
}

static void show(void) 
{
			gpio_set(GPIOA, GPIO15);
//	for(dly = 0; dly < 500000; dly++);
		__asm__("nop");
		__asm__("nop");
			gpio_clear(GPIOA, GPIO15);

}



void disp_spi_col_set(uint8_t data)
{
//bitbang_send(data);
//my_spi_send(data);

spi_send(SPI1, data);
/* wait for sending */
while (SPI_SR(SPI1) & SPI_SR_BSY && !(SPI_SR(SPI1) & SPI_SR_TXE));

/* FIXME make this with interrupt? */
show();
}

void disp_spi_init(void)
{

my_spi_init();
//bitbang_init();
stcp_init();
#if 0
#endif 
//bitbang_send(0xCA);
//show();
//	for(dly = 0; dly < 10000000; dly++);
//bitbang_send(0xAC);
//show();
}
