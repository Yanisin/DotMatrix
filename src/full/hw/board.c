#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>
#include <ch.h>
#include <assert.h>
#include <chibi/hal_st.h>
#include <hw/console.h>
#include <hw/disp.h>
#include <util/icons.h>
#include "hw_defs.h"
#include "board.h"

extern uint8_t _psp_base;

void __assert (const char * file , int line, const char * e)
{
	console_printf("assertion fail in %s:%d\n", file, line);
	board_halt(e);
}

void __assert_func (const char * file, int line, const char *func, const char *e)
{
	console_printf("assertion fail in %s:%d, %s\n", file, line, func);
	board_halt(e);
}

static void fill_stack(void)
{
	uint8_t *sp;
	__asm__ volatile ("mov %0, SP"  : "=r" (sp));
	for(; sp != &_psp_base; sp--)
		*sp = CH_DBG_STACK_FILL_VALUE;
	*sp = CH_DBG_STACK_FILL_VALUE;
}

void board_init(void)
{
	fill_stack();
	rcc_clock_setup_in_hsi_out_48mhz();
	rcc_periph_clock_enable(RCC_GPIOC);

	/* Set GPIO0 (in GPIO port F) to 'output push-pull'. */
	/* Using API functions: */
	gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
	stInit();
}

void board_halt(const char*msg)
{
	/* The rest is just a best effort --  we might be in a really bad shape */
	console_printf("halt reason: %s\n", msg);
	draw_icon(smiley_sad, BLIT_SET);
	__asm volatile("bkpt #0\n");
	while(1)
		;
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
