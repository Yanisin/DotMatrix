#pragma once
#ifndef ASSEMBLER
#include "../board.h"
#include <libopencmsis/core_cm3.h>
#include "cmsis_compat.h"
#endif

/* ChibOS's cmparams would like to pull in CMSIS and what not */
#define CORTEX_MODEL            0
#define CORTEX_HAS_FPU          0
#define CORTEX_PRIORITY_BITS    2
#define CORTEX_NUM_VECTORS      3

