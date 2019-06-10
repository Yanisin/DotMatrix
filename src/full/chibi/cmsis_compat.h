#pragma once
#include <libopencm3/cm3/nvic.h>

#define NVIC_PRIO_BITS 3
#define PendSV_IRQn 2

#define NMI_Handler nmi_handler

static inline void __set_PSP(uint32_t stack)
{
    __asm__ volatile ("msr PSP, %0" : : "r" (stack));
}

static inline uint32_t __get_PSP(void)
{
    register uint32_t result;
     __asm__ volatile ("mrs %0, PSP"  : "=r" (result));
     return result;
}

static inline void NVIC_SetPriority(uint8_t irq, uint32_t priority)
{
    nvic_set_priority(irq, priority << (8u - NVIC_PRIO_BITS));
}

static inline uint32_t __get_PRIMASK(void)
{
    register uint32_t result;
     __asm__ volatile ("mrs %0, PRIMASK"  : "=r" (result));
     return result;
}

static inline uint32_t __get_IPSR(void)
{
    register uint32_t result;
     __asm__ volatile ("mrs %0, IPSR"  : "=r" (result));
     return result;
}
