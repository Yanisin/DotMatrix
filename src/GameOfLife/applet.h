#ifndef _APPLET_H
#define _APPLET_H

struct applet {
	void (*init)(void);
	void (*worker)(void);
};

#define applet_add(name) \
	const struct applet *applet_ ## name \
	__attribute__((section (".applet."#name))) = &name ## _applet

extern void applet_init_all(void);
extern void applet_run_all(void);

#endif
