#ifndef TICKER_H
#define TICKER_H

#include <stdint.h>

extern void ticker_init(void);
extern void ticker_print_status(void);
extern void ticker_msleep(uint32_t msec);
extern unsigned int ticker_get_ticks(void);

static inline void cpu_relax(void)
{
#ifndef SIM
	__asm__ volatile("wfi");
#endif
}
#endif /* TICKER_H */
