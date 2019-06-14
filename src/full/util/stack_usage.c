/* Use with debugger */
#include <ch.h>

/**
 * @file
 *  Stack usage analysis. Intended to be called from GDB. See the README.md.
 */

#if CH_DBG_ENABLE_STACK_CHECK == TRUE
struct thrinfo {
	const char *name;
	uint16_t used_percent;
	uint16_t size;
	uint16_t left;
};

#define THRINFO_COUNT 10

struct thrinfo stat_thrinfo[THRINFO_COUNT];

void stat_threads(void);

extern uint8_t _psp_stack;

void stat_threads(void)
{
	thread_t *tp;
	struct thrinfo *thi = stat_thrinfo;
	tp = chRegFirstThread();
	do {
		uint8_t *bottom = (uint8_t*)tp->wabase;
		uint8_t *top = (uint8_t*) tp;
		uint8_t *touched = (uint8_t *)bottom;
		if (thi == stat_thrinfo) {
			/* main thread is special */
			top = &_psp_stack;
		}

		while(touched < top && *touched == CH_DBG_STACK_FILL_VALUE)
			touched++;

		thi->name = tp->name;
		thi->size = top - bottom;
		thi->left = touched - bottom;
		thi->used_percent = ((top - touched) * 100)/thi->size;
		tp = chRegNextThread(tp);
		thi++;
	} while (tp != NULL && thi < stat_thrinfo + THRINFO_COUNT);

}
#endif
