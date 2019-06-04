#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "ticker.h"
#include "led.h"
#include "hw_defs.h"

#ifndef SIM
void led_init(void)
{
	/* Enable GPIOC clock. */
	rcc_periph_clock_enable(RCC_GPIOC);

	/* Set GPIO0 (in GPIO port F) to 'output push-pull'. */
	/* Using API functions: */
	gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
}

void led_on(void)
{
	gpio_clear(LED_PORT, LED_PIN);	/* LED on */
}

void led_off(void)
{
	gpio_set(LED_PORT, LED_PIN);	/* LED on */
}

void led_toggle(void)
{
	gpio_toggle(LED_PORT, LED_PIN);	/* LED on */
}
#endif

void led_blink_background(uint32_t cnt, uint32_t dly)
{
	/*FIXME finish this*/
	cnt=cnt;
	dly=dly;
}

void led_blink(uint32_t cnt, uint32_t dly)
{
	uint32_t i;
	for (i=0;i<cnt;i++) {
		led_on();
		ticker_msleep(dly);
		led_off();
		ticker_msleep(dly);
	}
}


