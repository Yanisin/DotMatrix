#pragma once

#include <ch.h>

static inline sysinterval_t timeout_until(systime_t abs)
{
	systime_t now = chVTGetSystemTimeX();
	if (abs <= now)
		return TIME_IMMEDIATE;
	else
		return abs - now;
}
