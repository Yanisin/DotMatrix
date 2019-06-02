#ifndef COMMON_GPIO_H
#define COMMON_GPIO_H

#include <stdbool.h>

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

#endif // COMMON_GPIO_H
