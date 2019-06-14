#pragma once

#include <ch.h>

/**
 * Compute a timeout remaining to a certain absolute time.
 *
 * If the time has already passed, TIME_IMMEDIATE is used
 */
static inline sysinterval_t timeout_until(systime_t abs)
{
	systime_t now = chVTGetSystemTimeX();
	if (abs <= now)
		return TIME_IMMEDIATE;
	else
		return abs - now;
}
