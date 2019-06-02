#ifdef SIM
#include <time.h>
#else
#include <libopencm3/cm3/systick.h>
#endif
#include "rand.h"
#include "cell_id.h"

static uint32_t random[1];

void rand_init(void)
{
#ifdef SIM
	uint32_t acc = 0;
	for(int i = 0; i<CELL_ID_LEN; i++) {
		acc = acc*31 ^ get_cell_id()->bytes[0];
	}
	random[0] = time(NULL) ^ acc;
#else
	random[0] = STK_CVR;
#endif
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
