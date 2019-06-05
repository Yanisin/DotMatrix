#include "int.h"
#include <libopencm3/cm3/cortex.h>

void int_disable(int_state *state)
{
	__asm__ volatile("": : :"memory");
	*state = cm_is_masked_interrupts();
	cm_disable_interrupts();
}

void int_restore(const int_state *state)
{
	if (!(*state)) {
		cm_enable_interrupts();
	}
}

bool int_is_disabled(void)
{
	return cm_is_masked_interrupts();
}

void cpu_relax(void)
{
	__asm__ volatile("wfi");
}
