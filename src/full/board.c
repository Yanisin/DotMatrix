#include "board.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>
#include "hw_defs.h"
#include "chibi/hal_st.h"
#include "console.h"
#include "ch.h"
#include "icons.h"
#include "disp.h"

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
	/* TODO: terminate display test if in progress */
	console_printf("halt: %s\n", msg);
	__asm volatile("bkpt #0\n");
	draw_icon(smiley_sad, BLIT_SET);
	chThdTerminate(display_test);
	chThdSleep(TIME_INFINITE);
}

void hard_fault_handler()
{
	/* https://www.element14.com/community/thread/54959/l/gdb-assisted-debugging-of-hard-faults?displayFullThread=true */
	/* In short, GDB can not yet unwind alternate stack faults. Let's fake the stack */
	uint32_t lr, psp, msp, sp;
	__asm__ volatile (
		"mrs %0, psp\n"
		"mrs %1, msp\n"
		"mov %2, lr\n"
		:"=r" (psp), "=r" (msp), "=r" (lr));
	if ((lr & 0xF) == 0xD)
		sp = psp;
	else
		sp = msp;

	__asm volatile
	(
		"mov sp, %0\n"
		"bkpt #0\n"
		: :"r" (sp));
	while(1)
		;
}
