#include "stdint.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include "ticker.h"

/*
 * system clock should be 48MHz
 * but what bout internal?
 */

static volatile uint32_t cnt;
static void ticker_tick(void)
{
	cnt++;
}

void sys_tick_handler(void)
{
	ticker_tick();
}

void ticker_msleep(uint32_t msec)
{
	uint32_t end;
	end = cnt + msec;
	while (cnt < end)
		;
}

unsigned int ticker_get_ticks(void)
{
	return cnt;
}

void ticker_init(void)
{
	cnt = 0;
	/* set system ticker to fire every 1ms */
	systick_set_reload(rcc_ahb_frequency / 8 / 1000);
	systick_counter_enable();
	systick_interrupt_enable();

	/* clear counter so it starts right away */
	STK_CVR = 0;
}
