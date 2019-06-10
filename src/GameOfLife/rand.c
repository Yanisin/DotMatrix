#include <libopencm3/cm3/systick.h>

#include "rand.h"
#include "ticker.h"

static uint32_t random[1];

void rand_init(void)
{
	random[0] = STK_CVR;
}

void rand_tick(void)
{
}

void rand_worker(void)
{
}

static uint32_t xorshift32(uint32_t state[static 1])
{
	/* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	uint32_t x = state[0];
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	state[0] = x;
	return x;
}

uint32_t rand_get(void)
{
	return xorshift32(random);
}
