#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ch.h>
#include <util/simple_printf.h>
#include <hw/cdcacm.h>
#include "console.h"

static bool console_unresponsive;

static void console_putc_single(const char c)
{
	sysinterval_t tout = console_unresponsive ? TIME_IMMEDIATE : TIME_MS2I(100);
	if (oqPutTimeout(&cdcacm_tx_queue, c, tout) == MSG_TIMEOUT) {
		console_unresponsive = true;
	}
}

static void console_putc_nocheck(char c)
{
	console_putc_single(c);
	if (c == '\n')
		console_putc_single('\r');
}

/**
 * Checks the queue if the console because responsive from the last timeout.
 * @return is the console responsive?
 */
static bool check_console(void)
{
	if (console_unresponsive) {
		/* No locking needed */
		if (!oqIsFullI(&cdcacm_tx_queue))
			console_unresponsive = false;
	}
	return !console_unresponsive;
}

void console_putc(const char c)
{
	if (!check_console())
		return;
	console_putc_nocheck(c);
}


void console_puts(const char *buf)
{
	if (!check_console())
		return;
	for (unsigned int l = 0; l < strlen(buf); ++l) {
		console_putc_nocheck(buf[l]);
	}
}

static void cdacm_print_handler(char c, void *arg)
{
	(void)arg;
	console_putc_nocheck(c);
}

void console_printf(const char *format, ...)
{
	va_list arg;
	va_start(arg, format);
	if (!check_console())
		return;
	vfctprintf(cdacm_print_handler, NULL, format, arg);
	va_end(arg);
}
