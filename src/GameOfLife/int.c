#include "int.h"
#include <libopencm3/cm3/cortex.h>

void int_disable(int_state *state)
{
	asm volatile("": : :"memory");
	*state = cm_is_masked_interrupts();
	cm_disable_interrupts();
}

void int_restore(const int_state *state)
{
	if (!(*state)) {
		cm_enable_interrupts();
	}
}
