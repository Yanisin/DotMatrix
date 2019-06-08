#include "buttons.h"
#include <ch.h>
#include <assert.h>
#include <string.h>

#define BTN_COUNT 3

struct btn_state {
	bool state;
	uint8_t events;
	virtual_timer_t hold_timer;
};

static struct btn_state buttons[BTN_COUNT];
EVENTSOURCE_DECL(button_events_ready);
BSEMAPHORE_DECL(event_ready, 0);

bool button_get_state(enum button btn)
{
	return buttons[btn].state;
}

bool button_get_any_events(uint8_t *events_out, sysinterval_t timeout)
{
	uint8_t events[BTN_COUNT];
	if (buttons_get_events(BTN_COUNT, events, timeout)) {
		*events_out = events[BTN_ANY];
		return true;
	} else {
		return false;
	}
}

bool buttons_get_events(size_t button_count, uint8_t *events_out, sysinterval_t timeout)
{
	msg_t wait;
	assert(button_count == BTN_COUNT);
	(void)button_count;

	chSysLock();
	wait = chBSemWaitTimeoutS(&event_ready, timeout);
	if (wait == MSG_OK) {
		for(int i = 0; i < BTN_COUNT; i++) {
			events_out[i] = buttons[i].events;
			buttons[i].events = 0;
		}
	}
	chSysUnlock();
	return wait == MSG_OK;
}

static void hold_cb(void *btn_void)
{
	struct btn_state *b = btn_void;
	chSysLockFromISR();
	chVTResetI(&b->hold_timer);
	b->events |= BTN_EV_HOLD;
	chBSemSignalI(&event_ready);
	chEvtBroadcastI(&button_events_ready);
	chSysUnlockFromISR();
}

static bool button_isrI(enum button btn, bool state)
{
	struct btn_state *b = &buttons[btn];
	if (b->state == state)
		return false;
	b->state = state;
	if (state) {
		b->events |= BTN_EV_DOWN;
		chVTSetI(&b->hold_timer, BUTTON_HOLD_TRESHOLD, hold_cb, b);
	} else {
		b->events |= BTN_EV_UP;
		chVTResetI(&b->hold_timer);
	}
	/* Also handle the virtual button */
	button_isrI(BTN_ANY, buttons[BTN_0].state || buttons[BTN_1].state);
	return true;
}

void button_isr(enum button btn, bool state)
{

	chSysLockFromISR();
	if (button_isrI(btn, state)) {
		chBSemSignalI(&event_ready);
		chEvtBroadcastI(&button_events_ready);
	}
	chSysUnlockFromISR();
}

