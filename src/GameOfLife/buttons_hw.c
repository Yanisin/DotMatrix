#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include "hw_defs.h"
#include "applet.h"
#include "led.h"
#include "buttons_hw.h"

static void gpio_setup(void)
{
	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(BTN0_RCC);
	rcc_periph_clock_enable(BTN1_RCC);

	/*FIXME what about push up/down? */
	gpio_mode_setup(BTN0_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BTN0_PIN);
	gpio_mode_setup(BTN1_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BTN1_PIN);
}

static void btn_init(void)
{
	gpio_setup();
}

bool button_is_pressed(enum button b) {
	bool b0 = !gpio_get(BTN0_PORT, BTN0_PIN);
	bool b1 = !gpio_get(BTN1_PORT, BTN1_PIN);
	switch (b) {
	case BTN_1:
		return b0;
	case BTN_2:
		return b1;
	case BTN_ANY:
		return b0 || b1;
	default:
		return false;
	}
}


static const struct worker btn_worker = {
	.init = btn_init,
	.run = NULL,
};

worker_add(btn);
