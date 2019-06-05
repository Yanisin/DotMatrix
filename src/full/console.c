#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "console.h"
#include "cdcacm.h"

uint32_t console_getc(void)
{
	return cdcacm_get_char();
}

void console_putc(const char c)
{
	cdcacm_write_char(c);
	if (c == '\n')
		cdcacm_write_char('\r');
}

void console_puts(const char *buf)
{
	for (unsigned int l = 0; l < strlen(buf); ++l) {
		console_putc(buf[l]);
	}
}

#ifdef CONSOLE_PRINTF
void console_printf(const char *format, ...)
{
	   va_list arg;
	   char buf[64];

	   va_start(arg, format);
	   vsnprintf(buf, sizeof(buf), format, arg);
	   va_end(arg);

	   console_puts(buf);
}
#endif
