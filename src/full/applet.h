#ifndef _APPLET_H
#define _APPLET_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <ch.h>

struct usart_header_str;

struct applet {
	void (*run)(void);
	const uint8_t icon[8];
	int priority;
};

#define applet_add(name) \
	const struct applet *applet_ ## name \
	__attribute__((section (".applet."#name))) = &name ## _applet

struct worker {
	void (*init)(void);
	void (*run)(void);
};

#define worker_add(name) \
	const struct worker *worker_ ## name \
	__attribute__((section (".worker."#name))) = &name ## _worker

void worker_init_all(void);
void worker_run_all(void);

extern event_source_t applet_should_end_event;
extern bool applet_should_end;

const struct applet* applet_current(void);
void applet_select_local(const struct applet *applet);
void applet_select(uint8_t applet);
size_t applet_count(void);
const struct applet* applet_get(uint8_t index);
void applet_wait_for_end(sysinterval_t timeout);
uint8_t applet_topmost(void);


#endif
