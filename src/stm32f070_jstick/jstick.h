#ifndef JSTICK_H
#define JSTICK_H

uint32_t jstick_get_x(void);
uint32_t jstick_get_y(void);

void jstick_init(void);
void jstick_worker(void);

#endif
