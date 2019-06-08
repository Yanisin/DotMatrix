#include "applet.h"
#include "topo.h"
#include "mgmt_proto.h"
#include <assert.h>
#include "util.h"

extern const struct applet *__applet_array_start;
extern const struct applet *__applet_array_end;

extern const initializer_cb __dminit_array_start;
extern const initializer_cb __dminit_array_end;

static const struct applet* current;

EVENTSOURCE_DECL(applet_should_end_event);
MUTEX_DECL(lock);
bool applet_should_end;

const struct applet* applet_current(void)
{
	return current;
}

void applet_select_local(const struct applet *applet)
{
	chMtxLock(&lock);
	current = applet;
	applet_should_end = true;
	chEvtBroadcast(&applet_should_end_event);
	chMtxUnlock(&lock);
}

void applet_select(uint8_t applet)
{
	assert(topo_is_master);
	struct mgmt_change_applet msg;
	msg.applet = applet;

	applet_select_local(applet_get(applet));
	send_routed_msg(false, MGMT_CHANGE_APPLET, MSG_ROUTE_BROADCAST, sizeof(msg), &msg);
}

size_t applet_count(void)
{
	return &__applet_array_end - &__applet_array_start;
}

const struct applet* applet_get(uint8_t index)
{
	return (&__applet_array_start)[index];
}

uint8_t applet_topmost(void)
{
	int best = 0;
	for (size_t i = 0; i < applet_count(); i++) {
		best = MAX(best, applet_get(i)->priority);
	}
	for (size_t i = 0; i < applet_count(); i++) {
		if (applet_get(i)->priority == best)
			return i;
	}
	return 0;
}

void applet_wait_for_end(sysinterval_t timeout)
{
	event_listener_t end_listener;
	chEvtRegister(&applet_should_end_event, &end_listener, 0);
	chEvtWaitAnyTimeout(EVENT_MASK(0), timeout);
	chEvtUnregister(&applet_should_end_event, &end_listener);
}

void run_all_initializers(void)
{
	for (const initializer_cb *i = &__dminit_array_start; i != &__dminit_array_end; i++) {
		(*i)();
	}
}
