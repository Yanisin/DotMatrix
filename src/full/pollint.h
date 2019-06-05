#pragma once

#ifdef SIM
#include "sim/sim.h"
static inline void poll_int(void)
{
	bool dummy;
	sim_irq_wait(NULL, &dummy);
}
#else
static inline void poll_int(void)
{
}
#endif
