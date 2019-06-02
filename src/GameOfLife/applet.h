#ifndef _APPLET_H
#define _APPLET_H

#include <stddef.h>
#include <stdbool.h>

struct usart_header_str;

struct applet {
	void (*init)(void);
	void (*destroy)(void);
	void (*worker)(void);
	/**
	 * @returns false if the message is not recognized
	 */
	bool (*check_usart)(const struct usart_header_str *msg);
};

#define applet_add(name) \
	const struct applet *applet_ ## name \
	__attribute__((section (".applet."#name))) = &name ## _applet

const struct applet* applet_current(void);
void applet_select(const struct applet *applet);
size_t applet_count(void);
const struct applet* applet_get(int index);


#endif
