#include <libopencm3/cm3/vector.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/scb.h>

typedef void (*funcp_t) (void);
extern funcp_t __preinit_array_start, __preinit_array_end;
extern funcp_t __init_array_start, __init_array_end;
extern funcp_t __fini_array_start, __fini_array_end;

void crt_init(void);

void crt_init(void)
{
	volatile unsigned *src, *dest;
	funcp_t *fp;
	SCB_CCR |= SCB_CCR_STKALIGN;

	for (src = &_data_loadaddr, dest = &_data;
		dest < &_edata;
		src++, dest++) {
		*dest = *src;
	}

	while (dest < &_ebss) {
		*dest++ = 0;
	}

	/* Constructors. */
	for (fp = &__preinit_array_start; fp < &__preinit_array_end; fp++) {
		(*fp)();
	}
	for (fp = &__init_array_start; fp < &__init_array_end; fp++) {
		(*fp)();
	}
}
