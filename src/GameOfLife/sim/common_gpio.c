#include "../common_gpio.h"
#include <stdint.h>
#include "sim_proto.h"
#include "proto_defs.h"

void common_gpio_init(void)
{
}

void common_gpio_set(bool on)
{
	uint8_t state = on;
	sim_send(MSGID_GPIO_STATE, sizeof(&state), &state);
}

bool common_gpio_get()
{
	return sim_common_gpio;
}
