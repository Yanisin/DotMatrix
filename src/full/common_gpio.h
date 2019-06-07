#ifndef COMMON_GPIO_H
#define COMMON_GPIO_H

#include <stdbool.h>
#include <ch.h>

/**
 * Initialize the common GPIO.
 *
 * The GPIO is initialized as open-drain with pullup. The result is a logical & of outputs
 * of all the boards. Initially, the the gpio is signal is in logical 1 => the board is
 * not driving the signal low.
 *
 * Called automatically on system start.
 */
void common_gpio_init(void);

/**
 * Driver the gpio high or leave it.
 */
void common_gpio_set(bool on);

/**
 * Read the state of shared GPIO rail.
 */
bool common_gpio_get(void);

/**
 * Wait for the common GPIO line to get in a certain state.
 *
 * @returns true if the line is in that state
 * @returns false if timeout occured
 */
bool common_gpio_wait_for(bool state, sysinterval_t timeout);

#define COMMON_GPIO_FLAG_LOW 0x1
#define COMMON_GPIO_FLAG_HIGH 0x2
extern event_source_t common_gpio_event;


#endif // COMMON_GPIO_H
