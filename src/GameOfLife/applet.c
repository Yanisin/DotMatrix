#include "applet.h"

extern const char __applet_array_start;
extern const char __applet_array_end;

void applet_init_all(void)
{
	const struct applet **array_start = (const struct applet **)&__applet_array_start;
	const struct applet **array_end = (const struct applet **)&__applet_array_end;

	for (const struct applet **p = array_start; p < array_end; ++p)
		if ((*p)->init)
			(*p)->init();
}

void applet_run_all(void)
{
	const struct applet **array_start = (const struct applet **)&__applet_array_start;
	const struct applet **array_end = (const struct applet **)&__applet_array_end;

	for (const struct applet **p = array_start; p < array_end; ++p)
		if ((*p)->worker)
			(*p)->worker();
}
