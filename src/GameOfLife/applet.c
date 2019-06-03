#include "applet.h"

extern const struct applet *__applet_array_start;
extern const struct applet *__applet_array_end;

extern const struct worker *__worker_array_start;
extern const struct worker *__worker_array_end;

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

void worker_init_all(void)
{
	for (const struct worker **w = &__worker_array_start; w != &__worker_array_end; w++) {
		if ((*w)->init)
			(*w)->init();
	}
}

void worker_run_all(void)
{
	for (const struct worker **w = &__worker_array_start; w != &__worker_array_end; w++) {
		if ((*w)->run)
			(*w)->run();
	}
}
