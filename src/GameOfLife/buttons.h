#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdbool.h>
#include "buttons_hw.h"

enum button_event {
	BTN_EV_NONE, BTN_EV_UP, BTN_EV_DOWN, BTN_EV_HOLD
};

enum button_event button_get_event(enum button btn);

#endif // BUTTONS_H
