#pragma once

#ifdef SIM
#include "sim/sim.h"
#include "ch.h"
static inline void poll_int(void)
{
	CH_IRQ_PROLOGUE();
	sim_irq_wait(false);
	CH_IRQ_EPILOGUE();

	chSysLock();
	if (chSchIsPreemptionRequired())
		chSchDoReschedule();
	chSysUnlock();
}
#else
static inline void poll_int(void)
{
}
#endif
