#ifndef DISP_H
#define DISP_H
#include "stdint.h"

void disp_init(void);
void disp_clean(void);
void disp_set(uint8_t col, uint8_t row, uint8_t val);


#endif

