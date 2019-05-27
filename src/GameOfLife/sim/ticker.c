#include "../ticker.h"
#include <unistd.h>
#include <time.h>

static struct timespec boot_time;

void ticker_init(void)
{
	clock_gettime(CLOCK_MONOTONIC, &boot_time);
}

void ticker_print_status(void)
{
}

void ticker_msleep(uint32_t msec)
{
	usleep(msec * 1000);
}

unsigned int ticker_get_ticks(void)
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	t.tv_sec -= boot_time.tv_sec;
	if (t.tv_nsec < boot_time.tv_nsec) {
		t.tv_nsec += 1000*1000*1000;
	}
	t.tv_nsec -= boot_time.tv_nsec;
	return t.tv_nsec / (1000*1000) + (t.tv_sec * 1000);
}
