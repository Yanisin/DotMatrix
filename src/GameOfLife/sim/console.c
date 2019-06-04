#include "../console.h"
#include "../cell_id.h"
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
	const cell_id_t* cid= get_cell_id();

	printf("%02x%02x: ", cid->bytes[CELL_ID_LEN - 2], cid->bytes[CELL_ID_LEN - 1]);
	vprintf(format, args);

	va_end(args);
}
