#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

#include "hw_defs.h"
#include "applet.h"
#include "led.h"
#include "buttons_hw.h"
#include "buttons.h"

static void btn_init(void)
{
	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(BTN0_RCC);
	rcc_periph_clock_enable(BTN1_RCC);

	/*FIXME what about push up/down? */
	gpio_mode_setup(BTN0_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BTN0_PIN);
	gpio_mode_setup(BTN1_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BTN1_PIN);

	/* Enable EXTI0 interrupt. */
	nvic_enable_irq(BTN_IRQ);

	/* Configure the EXTI subsystem. */
	exti_select_source(BTN0_EXTI, BTN0_PORT);
	exti_set_trigger(BTN0_EXTI, EXTI_TRIGGER_BOTH);
	exti_enable_request(BTN0_EXTI);

	/*FIXME make this via defines from hw_defs*/
	exti_select_source(BTN1_EXTI, BTN1_PORT);
	exti_set_trigger(BTN1_EXTI, EXTI_TRIGGER_BOTH);
	exti_enable_request(BTN1_EXTI);
}
init_add(btn_init);

void exti4_15_isr(void)
{
	CH_IRQ_PROLOGUE();
	if(exti_get_flag_status(BTN0_EXTI)){
		button_isr(BTN_1, !gpio_get(BTN0_PORT, BTN0_PIN));
		exti_reset_request(EXTI6);
	}

	if(exti_get_flag_status(BTN1_EXTI)) {
		button_isr(BTN_1, !gpio_get(BTN1_PORT, BTN1_PIN));
		exti_reset_request(EXTI7);
	}
	CH_IRQ_EPILOGUE();
}
