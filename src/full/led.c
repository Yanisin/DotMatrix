#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "led.h"
#include "hw_defs.h"
#include "applet.h"

#ifndef SIM
void led_init(void)
{
	/* Enable GPIOC clock. */
	rcc_periph_clock_enable(RCC_GPIOC);

	/* Set GPIO0 (in GPIO port F) to 'output push-pull'. */
	/* Using API functions: */
	gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
}
init_add(led_init);

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
