#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdbool.h>
#include <ch.h>
#include "buttons_hw.h"

#define BTN_EV_UP 0x1
#define BTN_EV_DOWN 0x2
#define BTN_EV_HOLD 0x4

#define BUTTON_HOLD_TRESHOLD TIME_MS2I(500)

extern event_source_t button_events_ready;

bool button_get_state(enum button btn);
bool button_get_any_events(uint8_t *events_out, sysinterval_t timeout);
bool buttons_get_events(size_t button_count, uint8_t *events_out, sysinterval_t timeout);
void button_isr(enum button btn, bool state);

#endif // BUTTONS_H
