#include "initializers.h"

extern const initializer_cb __dminit_array_start;
extern const initializer_cb __dminit_array_end;

void run_all_initializers(void)
{
	for (const initializer_cb *i = &__dminit_array_start; i != &__dminit_array_end; i++) {
		(*i)();
	}
}
