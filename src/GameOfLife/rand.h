#ifndef RAND_H
#define RAND_H
#include "stdint.h"

void rand_init(void);
void rand_tick(void);
void rand_worker(void);
uint32_t rand_get(void);

#endif
