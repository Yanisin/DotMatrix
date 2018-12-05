#ifndef TICKER_H
#define TICKER_H

extern void ticker_init(void);
extern void ticker_print_status(void);
extern void ticker_msleep(uint32_t msec);
extern unsigned int ticker_get_ticks(void);

#endif /* TICKER_H */
