#include "common_gpio.h"
#include <libopencm3/stm32/gpio.h>

void common_gpio_init(void)
{
	gpio_mode_setup(GPIOB,
		GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO8|GPIO9);
	gpio_set_output_options(GPIOB,
			GPIO_OTYPE_OD, GPIO_OSPEED_LOW, GPIO8|GPIO9);
}

void common_gpio_set(bool on)
{
	if (on) {
		gpio_set(GPIOB, GPIO8|GPIO9);
	} else {
		gpio_clear(GPIOB, GPIO8|GPIO9);
	}
}

bool common_gpio_get()
{
	return gpio_get(GPIOB, GPIO8|GPIO9) != 0;
}

bool common_gpio_wait_for(bool state, sysinterval_t timeout)
{
	/* TODO: fix */
	while(common_gpio_get() != state)
		port_wait_for_interrupt();
	return true;
}
