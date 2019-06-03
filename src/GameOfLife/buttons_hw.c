#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include "hw_defs.h"
#include "applet.h"
#include "led.h"


static void clock_setup(void)
{
	rcc_clock_setup_in_hsi_out_48mhz();
}

static void gpio_setup(void)
{
	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(BTN0_RCC);
	rcc_periph_clock_enable(BTN1_RCC);

	/*FIXME what about push up/down? */
	gpio_mode_setup(BTN0_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, BTN0_PIN);
	gpio_mode_setup(BTN1_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, BTN1_PIN);
}

static void exti_setup(void)
{
	/* Enable EXTI0 interrupt. */
	nvic_enable_irq(NVIC_EXTI4_15_IRQ);

	/* Configure the EXTI subsystem. */
	exti_select_source(EXTI6, GPIOA);
	exti_set_trigger(EXTI6, EXTI_TRIGGER_FALLING);
	exti_enable_request(EXTI6);
	/*FIXME make this via defines from hw_defs*/
	exti_select_source(EXTI7, GPIOA);
	exti_set_trigger(EXTI7, EXTI_TRIGGER_FALLING);
	exti_enable_request(EXTI7);
}

void exti4_15_isr(void)
{
	if(exti_get_flag_status(EXTI6)){
		exti_reset_request(EXTI6);

	if(exti_get_flag_status(EXTI7)) {
		exti_reset_request(EXTI7);
	}
#if 0
	if (exti_direction == FALLING) {
		gpio_set(LED_PORT, LED_PIN);
		exti_direction = RISING;
		exti_set_trigger(EXTI6, EXTI_TRIGGER_RISING);
	} else {
		gpio_clear(LED_PORT,LED_PIN);
		exti_direction = FALLING;
		exti_set_trigger(EXTI6, EXTI_TRIGGER_FALLING);
	}
#endif
}

static void btn_init(void)
{
	clock_setup();
	gpio_setup();
	exti_setup();
}


static const struct worker btn_worker = {
	.init = btn_init,
	.run = NULL,
};

worker_add(btn);
