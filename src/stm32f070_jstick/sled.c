#include "sled.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define LED_RCC RCC_GPIOC
#define LED_GPIO GPIOC
#define LED_PIN GPIO15

void sled_init(void)
{
	/* Enable GPIOC clock. */
	rcc_periph_clock_enable(LED_RCC);

	/* Set GPIO0 (in GPIO port F) to 'output push-pull'. */
	/* Using API functions: */
	gpio_mode_setup(LED_GPIO, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);

}
void sled_on(void)
{

		gpio_set(LED_GPIO, LED_PIN);	/* LED on */
}
void sled_off(void)
{

		gpio_clear(LED_GPIO, LED_PIN);	/* LED on */
}
void sled_toggle(void)
{

		gpio_toggle(LED_GPIO, LED_PIN);	/* LED on/off */
}


