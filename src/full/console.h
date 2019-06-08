#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <stdint.h>

#if defined(CONSOLE_PRINTF) || defined(SIM)
extern void console_printf(const char *format, ...);
#else
static inline void console_printf(const char *format, ...)
{
	(void)format;
}
#endif
extern void console_puts(const char *s);
extern void console_putc(const char c);
extern uint32_t console_getc(void);

#endif
