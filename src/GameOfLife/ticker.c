#include "stdint.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include "ticker.h"
#include "dots.h"

#ifdef HOST_EMU
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#endif

/*
 * system clock should be 48MHz
 * but what bout internal?
 */

static volatile uint32_t cnt;

static void ticker_tick(void);

#ifndef HOST_EMU

void sys_tick_handler(void)
{
	ticker_tick();
}
#else
/* FIXME
create a pthread
*/


pthread_t tid;

static void* tick(void *arg)
{
//	unsigned long i = 0;
	static uint32_t i;
	int c;
	pthread_t id = pthread_self();
#if 0
	if(pthread_equal(id,tid[0]))
	{
		printf("\n First thread processing\n");
	}
	else
	{
		printf("\n Second thread processing\n");
	}

	for(i=0; i<(0xFFFFFFFF);i++);
#endif
	while(1){
//		printf("baah\n");
		usleep(1000);
		ticker_tick();
	}
}


#endif

void ticker_msleep(uint32_t msec)
{
	uint32_t end;
	end = cnt + msec;
	/*FIXME what about lock? */
	/*FIXME what about overflow */
	while (cnt < end);

}

void ticker_init(void)
{
#ifndef HOST_EMU

	//send_str((unsigned char *)"SystemCoreClock(HCLK):");
	//send_u32(SystemCoreClock);
	//send_str((unsigned char *)"Hz\r\n");

	/* set system ticker to fire every 1ms */
	systick_set_reload(rcc_ahb_frequency / 8 / 1000);
	systick_counter_enable();
	systick_interrupt_enable();

	/* clear counter so it starts right away */
	STK_CVR = 0;

#else
/* FIXME create a pthread */
	int err;
	err = pthread_create(&(tid), NULL, &tick, NULL);
	if (err != 0)
		printf("\ncan't create thread :[%s]", strerror(err));

#endif
}

static void ticker_tick(void)
{
  cnt++;
//  conf_pwm_tick();
//  eval_tick();
//  leds_tick();
//  lights_tick();
	dots_tick();
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
