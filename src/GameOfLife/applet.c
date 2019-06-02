#include "applet.h"

extern const struct applet *__applet_array_start;
extern const struct applet *__applet_array_end;

static const struct applet* current;

const struct applet* applet_current(void)
{
	return current;
}

void applet_select(const struct applet *applet)
{
	if (current && current->destroy) {
		current->destroy();
	}
	current = applet;
	current->init();
}

size_t applet_count(void)
{
	return &__applet_array_end - &__applet_array_start;
}

const struct applet* applet_get(int index)
{
	return (&__applet_array_start)[index];
}
