#pragma once

#include <stdint.h>

/**
 * @file
 *
 * @remark These funcitons are not safe to call from ISRs. The functions are blocking.
 */

extern void console_printf(const char *format, ...);
extern void console_puts(const char *s);
extern void console_putc(const char c);
extern int console_getc(void);

