#include "../common_gpio.h"
#include <stdint.h>
#include "sim.h"
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

bool common_gpio_wait_for(bool state, sysinterval_t timeout)
{
	/* TODO: fix */
	while(common_gpio_get() != state)
		port_wait_for_interrupt();
	return true;
}
