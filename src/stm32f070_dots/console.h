#ifndef _CONSOLE_H
#define _CONSOLE_H

extern void console_printf(const char *format, ...);
extern void console_puts(const char *s);
extern void console_putc(const char c);
extern uint32_t console_getc(void);

#endif
