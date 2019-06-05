#include "hal_st.h"
#include <ch.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>

/**
 * @file Implements the System Timer (ST) fot ChibiOS
 *
 * Currently is systick based, for simplicity. Systick does not support tickless mode in Chibi.
 */

void sys_tick_handler(void)
{
	CH_IRQ_PROLOGUE();
	chSysLockFromISR();
	chSysTimerHandlerI();
	chSysUnlockFromISR();
	CH_IRQ_EPILOGUE();
}

void stInit(void)
{
	systick_set_reload(rcc_ahb_frequency / 8 / CH_CFG_ST_FREQUENCY);
	systick_interrupt_enable();
	systick_counter_enable();
	STK_CVR = 0;
}
