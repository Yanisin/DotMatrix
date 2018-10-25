#include "rand.h"

static uint32_t random;

void rand_init(void)
{
random = 1;
}
void rand_tick(void)
{
random = 7 * random % 11;
}

void rand_worker(void)
{

}

uint32_t rand_get(void)
{
random = 7 * random % 11;
return random;
}
