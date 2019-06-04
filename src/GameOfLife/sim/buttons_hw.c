#include "../buttons.h"
#include <string.h>
#include "sim_proto.h"

bool button_is_pressed(enum button btn)
{
	switch (btn) {
	case BTN_1:
		return sim_button_state[0];
	case BTN_2:
		return sim_button_state[1];
	case BTN_ANY:
		return sim_button_state[0] || sim_button_state[1];
	}
	return false;
}
