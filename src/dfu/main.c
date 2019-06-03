#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/syscfg.h>
#include <libopencm3/stm32/memorymap.h>
#include <stdint.h>
#include <stddef.h>
#include "usart_buffered.h"
#include "usb.h"
#include "main.h"
#include <memory.h>

#define DFU_TIMEOUT 1000

static uint32_t tick_count;
static bool dfu_entered;
int page_number;
uint8_t page_data[PAGE_SIZE] __attribute__ ((aligned (4)));

struct vectors_header {
	uint32_t stack;
	uint32_t reset;
};

void sys_tick_handler(void)
{
	tick_count++;
}

void panic(void)
{
	while(1)
		;
}

void enter_dfu(void)
{
	dfu_entered = true;
}

static void ticker_init(void)
{
	/* set system ticker to fire every 1ms */
	systick_set_reload(rcc_ahb_frequency / 8 / 1000);
	systick_counter_enable();
	systick_interrupt_enable();

	/* clear counter so it starts right away */
	STK_CVR = 0;
}

static void ticker_teardown(void)
{
	systick_interrupt_disable();
	systick_counter_disable();
}

void handle_page(void)
{
	uintptr_t page = FLASH_BASE + PAGE_SIZE * (FLASH_RESERVED_PAGES + page_number);
	if (memcmp((void*)page, page_data, PAGE_SIZE) == 0)
		return;
	flash_unlock();
	flash_erase_page(page);
	for (size_t i = 0; i < PAGE_SIZE; i+= 4) {
		flash_program_word(page + i, *(uint32_t*)(page_data + i));
	}

	flash_lock();
	if (*(uint32_t*) (FLASH_BASE + PAGE_SIZE * (FLASH_RESERVED_PAGES)) == 0xFFFFFFFF) {
		panic();
	}
}

void continue_boot(void)
{
	usb_teardown();
	ticker_teardown();

	/* Copy the vectors to RAM, since we can map that at correct location */
	void *ram = (void*) 0x20000000;
	void *user_code = (void*) (FLASH_BASE + PAGE_SIZE * FLASH_RESERVED_PAGES);
	memcpy(ram, user_code, VECTOR_TABLE_SIZE);
	struct vectors_header *v = user_code;
	uint32_t stack = v->stack;
	uint32_t reset = v->reset;
	uint32_t syscfgr = SYSCFG_CFGR1;
	syscfgr &= ~SYSCFG_CFGR1_MEM_MODE;
	syscfgr |= SYSCFG_CFGR1_MEM_MODE_SRAM;
	SYSCFG_CFGR1 = syscfgr;

	/* jump & change stack */
	__asm__ __volatile__(
	    "mov sp, %[sp]\n"
	    "bx %[pc]"
	    :: [sp] "r" (stack), [pc] "r" (reset));
}

int main(void) {
	rcc_clock_setup_in_hsi_out_48mhz();
	ticker_init();
	//usart_init();
	usb_init();

	while(dfu_entered || tick_count < DFU_TIMEOUT) {
		usb_run();
	}

	continue_boot();
}
