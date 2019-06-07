#include "board.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "hw_defs.h"
#include "chibi/hal_st.h"
#include "console.h"
#include "ch.h"
#include "icons.h"

void board_init(void)
{
	rcc_clock_setup_in_hsi_out_48mhz();
	rcc_periph_clock_enable(RCC_GPIOC);

	/* Set GPIO0 (in GPIO port F) to 'output push-pull'. */
	/* Using API functions: */
	gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
	stInit();
}

void board_halt(const char*msg)
{
	console_printf("halt: %s\n", msg);
	draw_icon(smiley_sad, false);
	chThdSleep(TIME_INFINITE);
}
