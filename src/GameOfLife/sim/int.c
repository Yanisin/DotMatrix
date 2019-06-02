#include "../int.h"
#include "sim_proto.h"

static bool disabled;

void int_disable(int_state *state)
{
	sim_int_disable(state);
	disabled = true;
}

void int_restore(const int_state *state)
{
	disabled = *state;
	sim_int_restore(state);
}

bool int_is_disabled(void)
{
	return disabled;
}
