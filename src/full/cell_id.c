#include "cell_id.h"
#include <libopencm3/stm32/f0/memorymap.h>

const cell_id_t* get_cell_id(void)
{
	return (const cell_id_t*)DESIG_UNIQUE_ID_BASE;
}
