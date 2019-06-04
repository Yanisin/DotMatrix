#include "applet.h"
#include "topo.h"
#include "mgmt_msg.h"

extern const struct applet *__applet_array_start;
extern const struct applet *__applet_array_end;

extern const struct worker *__worker_array_start;
extern const struct worker *__worker_array_end;

static const struct applet* current;

const struct applet* applet_current(void)
{
	return current;
}

static void applet_init(void) {
	if (current->init)
		current->init();
}

static void applet_destroy(void) {
	if (current && current->destroy) {
		current->destroy();
	}
}

void applet_select_local(const struct applet *applet)
{
	applet_destroy();
	current = applet;
	applet_init();
}

void  applet_select(uint8_t applet)
{
	struct mgmt_change_applet msg;
	msg.applet = applet;

	applet_destroy();
	current = applet_get(applet);
	topo_send_down(MGMT_CHANGE_APPLET, sizeof(msg), &msg);
	applet_init();
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
