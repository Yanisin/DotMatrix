#ifndef INT_H
#define INT_H

#include <stdbool.h>

typedef int int_state;

void int_disable(int_state *state);
void int_restore(const int_state *state);
bool int_is_disabled(void);

#endif // INT_H
