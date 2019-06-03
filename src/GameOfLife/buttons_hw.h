#ifndef BUTTONS_HW
#define BUTTONS_HW

#include <stdbool.h>

enum button {
	BTN_1, BTN_2, BTN_ANY
};

bool button_is_pressed(enum button btn);

#endif
