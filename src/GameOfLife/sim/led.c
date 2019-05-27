#include "../led.h"
#include <stdbool.h>
#include "sim_proto.h"
#include "proto_defs.h"

static bool led_state;

static void send_led_state(void)
{
	uint8_t state = led_state;
	sim_send(MSGID_LED_STATE, sizeof(&state), &state);
}

void led_init(void)
{
}

void led_on(void)
{
	led_state = true;
	send_led_state();
}

void led_off(void)
{
	led_state = false;
	send_led_state();
}

void led_toggle(void)
{
	led_state = !led_state;
	send_led_state();
}
