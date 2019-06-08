#ifndef BUTTONS_HW
#define BUTTONS_HW

#include <stdbool.h>

enum button {
	BTN_0, BTN_1, BTN_ANY, BTN_COUNT
};

bool button_is_pressed(enum button btn);

#endif
