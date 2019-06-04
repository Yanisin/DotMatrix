#include "buttons.h"
#include "ticker.h"

#define BTN_COUNT 3

struct btn_state {
	bool last_state;
	unsigned int change_time;
	bool hold_reported;
};

static struct btn_state button_state[BTN_COUNT];

enum button_event button_get_event(enum button btn)
{
	struct btn_state *b = &button_state[btn];
	enum button_event evt = BTN_EV_NONE;
	unsigned int now = ticker_get_ticks();
	bool state = button_is_pressed(btn);
	if (state != b->last_state) {
		evt = state ? BTN_EV_DOWN : BTN_EV_UP;
		b->last_state = state;
		b->change_time = now;
		b->hold_reported = false;
	} else if (state && now - b->change_time > 1000 && !b->hold_reported) {
		evt = BTN_EV_HOLD;
		b->hold_reported = true;
	}
	return evt;
}
