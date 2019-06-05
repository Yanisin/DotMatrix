/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    SIMIA32/chcore.c
 * @brief   Simulator on IA32 port code.
 *
 * @addtogroup SIMIA32_GCC_CORE
 * @{
 */

#include <sys/time.h>
#include "ch.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

static struct timespec next_tick;

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

bool port_isr_context_flag;
syssts_t port_irq_sts;

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

#define NS (1000*1000*1000)
static void advance_tick(void)
{
	next_tick.tv_nsec += NS/CH_CFG_ST_FREQUENCY;
	while (next_tick.tv_nsec >= NS) {
		next_tick.tv_nsec -= NS;
		next_tick.tv_sec += 1;
	}
}

static bool is_before(const struct timespec *a, const struct timespec *b)
{
	return (a->tv_sec < b->tv_sec) ||
		((a->tv_sec == b->tv_sec) && (a->tv_nsec < b->tv_nsec));
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void port_init(void)
{
	port_irq_sts = (syssts_t)0;
	port_isr_context_flag = false;
	clock_gettime(CLOCK_MONOTONIC, &next_tick);
	advance_tick();
}

void port_wait_for_interrupt(void)
{
	bool resched = false;
	CH_IRQ_PROLOGUE();
	chSysLockFromISR();

	struct timespec now;
	sim_irq_wait(&next_tick, &resched);

	clock_gettime(CLOCK_MONOTONIC, &now);
	if (is_before(&next_tick, &now)) {
		resched = true;
		advance_tick();
		chSysTimerHandlerI();
	}
	chSysUnlockFromISR();
	CH_IRQ_EPILOGUE();
	if (chSchIsPreemptionRequired())
		chSchDoReschedule();

}

/**
 * Performs a context switch between two threads.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched in
 */
__attribute__((used))
static void __dummy(thread_t *ntp, thread_t *otp) {
  (void)ntp; (void)otp;

  asm volatile (
#if defined(WIN32)
                ".globl @port_switch@8                          \n\t"
                "@port_switch@8:"
#elif defined(__APPLE__)
                ".globl _port_switch                            \n\t"
                "_port_switch:"
#else
                ".globl port_switch                             \n\t"
                "port_switch:"
#endif
                "push    %ebp                                   \n\t"
                "push    %esi                                   \n\t"
                "push    %edi                                   \n\t"
                "push    %ebx                                   \n\t"
                "movl    %esp, 12(%edx)                         \n\t"
                "movl    12(%ecx), %esp                         \n\t"
                "pop     %ebx                                   \n\t"
                "pop     %edi                                   \n\t"
                "pop     %esi                                   \n\t"
                "pop     %ebp                                   \n\t"
                "ret");
}

/**
 * @brief   Start a thread by invoking its work function.
 * @details If the work function returns @p chThdExit() is automatically
 *          invoked.
 */
__attribute__((cdecl, noreturn))
void _port_thread_start(msg_t (*pf)(void *), void *p) {

  chSysUnlock();
  pf(p);
  chThdExit(0);
  while(1);
}


/**
 * @brief   Returns the current value of the realtime counter.
 *
 * @return              The realtime counter value.
 */
rtcnt_t port_rt_get_counter_value(void) {
#if defined(WIN32)
  LARGE_INTEGER n;

  QueryPerformanceCounter(&n);

  return (rtcnt_t)(n.QuadPart / 1000LL);
#else
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return ((rtcnt_t)tv.tv_sec * (rtcnt_t)1000000) + (rtcnt_t)tv.tv_usec;
#endif
}

/** @} */
