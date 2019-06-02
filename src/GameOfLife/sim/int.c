#include "../int.h"
#include "sim_proto.h"

static int disable_nesting;

void int_disable(int_state *state)
{
	disable_nesting++;
	sim_int_disable(state);
}

void int_restore(const int_state *state)
{
	disable_nesting--;
	sim_int_restore(state);
}

bool int_is_disabled(void)
{
	return disable_nesting != 0;
}

void cpu_relax(void)
{
	sim_irq_wait();
}
