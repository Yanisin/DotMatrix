#pragma once

#ifndef ASSEMBLER
#include "../board.h"
#endif

#define CH_CFG_TIME_QUANTUM                 0
/* Manage all memory */
#define CH_CFG_MEMCORE_SIZE                 0
#define CH_CFG_NO_IDLE_THREAD               FALSE
#define CH_CFG_OPTIMIZE_SPEED               TRUE
#define CH_CFG_USE_REGISTRY                 TRUE
#define CH_CFG_USE_WAITEXIT                 TRUE
#define CH_CFG_USE_SEMAPHORES               TRUE
#define CH_CFG_USE_SEMAPHORES_PRIORITY      FALSE
#define CH_CFG_USE_MUTEXES                  TRUE
#define CH_CFG_USE_MUTEXES_RECURSIVE        FALSE
#define CH_CFG_USE_CONDVARS                 TRUE
#define CH_CFG_USE_CONDVARS_TIMEOUT         TRUE
#define CH_CFG_USE_EVENTS                   TRUE
#define CH_CFG_USE_EVENTS_TIMEOUT           TRUE
#define CH_CFG_USE_MESSAGES                 TRUE
#define CH_CFG_USE_MESSAGES_PRIORITY        FALSE
#define CH_CFG_USE_MAILBOXES                TRUE
#define CH_CFG_USE_MEMCORE                  TRUE
#define CH_CFG_USE_HEAP                     TRUE
#define CH_CFG_USE_MEMPOOLS                 TRUE
#define CH_CFG_USE_OBJ_FIFOS                TRUE
#define CH_CFG_USE_PIPES                    TRUE
#define CH_CFG_USE_DYNAMIC                  FALSE
#define CH_CFG_USE_FACTORY                  FALSE
#define CH_CFG_FACTORY_OBJECTS_REGISTRY     TRUE
#define CH_CFG_FACTORY_GENERIC_BUFFERS      TRUE
#define CH_CFG_FACTORY_SEMAPHORES           TRUE
#define CH_CFG_FACTORY_MAILBOXES            TRUE
#define CH_CFG_FACTORY_OBJ_FIFOS            TRUE
#define CH_CFG_FACTORY_PIPES                TRUE
#define CH_CFG_FACTORY_MAX_NAMES_LENGTH     20


#ifdef DEBUG
#define CH_DBG_SYSTEM_STATE_CHECK           TRUE
#define CH_DBG_ENABLE_CHECKS                TRUE
#define CH_DBG_ENABLE_ASSERTS               TRUE
#define CH_DBG_TRACE_MASK                   CH_DBG_TRACE_MASK_DISABLED
#define CH_DBG_TRACE_BUFFER_SIZE            128
#define CH_CFG_USE_TM                       FALSE
#define CH_DBG_STATISTICS                   FALSE
#ifdef SIM
#define CH_DBG_ENABLE_STACK_CHECK           FALSE
#else
#define CH_DBG_ENABLE_STACK_CHECK           TRUE
#endif
#define CH_DBG_FILL_THREADS                 TRUE
#define CH_DBG_THREADS_PROFILING            FALSE
#else
#define CH_CFG_USE_TM                       FALSE
#define CH_DBG_STATISTICS                   FALSE
#define CH_DBG_SYSTEM_STATE_CHECK           FALSE
#define CH_DBG_ENABLE_CHECKS                FALSE
#define CH_DBG_ENABLE_ASSERTS               FALSE
#define CH_DBG_TRACE_MASK                   CH_DBG_TRACE_MASK_DISABLED
#define CH_DBG_TRACE_BUFFER_SIZE            128
#define CH_DBG_ENABLE_STACK_CHECK           TRUE
#define CH_DBG_FILL_THREADS                 TRUE
#define CH_DBG_THREADS_PROFILING            FALSE
#endif

/* Hooks */
#define CH_CFG_SYSTEM_EXTRA_FIELDS
#define CH_CFG_SYSTEM_INIT_HOOK() {                                         \
}
#define CH_CFG_THREAD_EXTRA_FIELDS
#define CH_CFG_THREAD_INIT_HOOK(tp) {                                       \
}
#define CH_CFG_THREAD_EXIT_HOOK(tp) {                                       \
}
#define CH_CFG_CONTEXT_SWITCH_HOOK(ntp, otp) {                              \
}
#define CH_CFG_IRQ_PROLOGUE_HOOK() {                                        \
}
#define CH_CFG_IRQ_EPILOGUE_HOOK() {                                        \
}
#define CH_CFG_IDLE_ENTER_HOOK() {                                          \
}
#define CH_CFG_IDLE_LEAVE_HOOK() {                                          \
}

#define CH_CFG_IDLE_LOOP_HOOK() {                                           \
}
#define CH_CFG_SYSTEM_TICK_HOOK() {                                         \
}
#define CH_CFG_SYSTEM_HALT_HOOK(reason) board_halt(reason)

#define CH_CFG_TRACE_HOOK(tep) {                                            \
}

