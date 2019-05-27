#include "../console.h"
#include <stdio.h>

extern void console_puts(const char *s)
{
	puts(s);
}

extern void console_putc(const char c)
{
	putchar(c);
}
