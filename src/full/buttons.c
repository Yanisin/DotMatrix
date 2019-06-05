#include "buttons.h"
#include <ch.h>

#define BTN_COUNT 3

struct btn_state {
	bool last_state;
	systime_t change_time;
	bool hold_reported;
};

static struct btn_state button_state[BTN_COUNT];

enum button_event button_get_event(enum button btn)
{
	struct btn_state *b = &button_state[btn];
	enum button_event evt = BTN_EV_NONE;
	systime_t now = chVTGetSystemTime();
	bool state = button_is_pressed(btn);
	if (state != b->last_state) {
		evt = state ? BTN_EV_DOWN : BTN_EV_UP;
		b->last_state = state;
		b->change_time = now;
		b->hold_reported = false;
	} else if (state && (now - b->change_time) > TIME_MS2I(1000) && !b->hold_reported) {
		evt = BTN_EV_HOLD;
		b->hold_reported = true;
	}
	return evt;
}
