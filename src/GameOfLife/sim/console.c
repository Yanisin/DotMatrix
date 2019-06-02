#include "../console.h"
#include <stdio.h>
#include <stdarg.h>

void console_puts(const char *s)
{
	puts(s);
}

void console_putc(const char c)
{
	putchar(c);
}


uint32_t console_getc(void)
{
	return -1;
}

void console_printf(const char* format, ...)
{
	va_list args;
	va_start(args, format);

	vprintf(format, args);

	va_end(args);
}
