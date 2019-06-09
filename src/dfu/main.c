#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/syscfg.h>
#include <libopencm3/stm32/memorymap.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "usart_tx.h"
#include "usb.h"
#include "main.h"
#include "disp.h"
#include "i2c.h"
#include "hw_defs.h"

#define DFU_DEBUG

/* During the quiet phase, we request others to reboot, but we do not broadcast on the i2c lines */
#define QUIET_PHASE_END 400

/* During this phase, everyone has pullup on the SDA and SCL lines.
 * Master drives the SCL line low, which allows the others to sense the SDA/SCL lines.
 * Master is the cell that was enumerated over USB.
 */
#define LINE_DETECT_SAMPLE 1000
#define LINE_DETECT_PHASE_END 1600
/* During this phase, I2C is activated on master. On slaves it is activated only if they detected
 * the SDA/SCL crossover.
 *
 * If this phase passes without the user starting a USB download, the master sends a release command
 * to everyone on the bus.
 */
#define USB_WAIT_PHASE_END 2000

#define LED_RCC RCC_GPIOC
#define LED_PORT GPIOC
#define LED_PIN GPIO13
#define DISP_nOE_ROW_GPIO GPIOB
#define DISP_nOE_COL_GPIO GPIOB
#define DISP_nOE_ROW_GPIO_PIN GPIO2
#define DISP_nOE_COL_GPIO_PIN GPIO4


volatile uint32_t tick_count;
static uint32_t serviced_tick;
int page_number;
uint8_t page_data[PAGE_SIZE] __attribute__ ((aligned (4)));
static bool usb_disabled;
static int i2c_crossover;
static bool standalone;
bool dfu_activated;
bool master;

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

static void init_i2c_line_detect(void)
{
	gpio_set(I2C1_GPIO_PORT, I2C1_GPIO_SCL | I2C1_GPIO_SDA);
	gpio_set_output_options(I2C1_GPIO_PORT,
		GPIO_OTYPE_OD, GPIO_OSPEED_LOW, I2C1_GPIO_SCL | I2C1_GPIO_SDA);
	gpio_mode_setup(I2C1_GPIO_PORT,
		GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, I2C1_GPIO_SCL | I2C1_GPIO_SDA);
	if (master)
		gpio_clear(I2C1_GPIO_PORT, I2C1_GPIO_SCL);
}

void continue_boot(void)
{
	if (master)
		i2c_send_release();

	usb_teardown();
	ticker_teardown();
	i2c_teardown();
	rcc_periph_clock_disable(DISP_GPIOS_RCC);
	rcc_periph_clock_disable(I2C1_GPIO_RCC);
	rcc_periph_clock_disable(I2C2_GPIO_RCC);
	rcc_periph_clock_disable(RCC_I2C1);
	rcc_periph_clock_disable(RCC_I2C2);
	rcc_periph_clock_disable(LED_RCC);


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
	rcc_periph_clock_enable(I2C1_GPIO_RCC);
	rcc_periph_clock_enable(I2C2_GPIO_RCC);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_I2C1);
	rcc_periph_clock_enable(RCC_I2C2);
	rcc_periph_clock_enable(LED_RCC);
	/* turn on LED to indicate that we have power */
	gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
	gpio_set(LED_PORT, LED_PIN);
	ticker_init();
	disp_init();
	disp_show(icon_wait);
	usart_init();
	usb_init();

	usart_reset_neigbors();

	/* Continue until time-out. If we are a slave or the user has started download,
	 * there is not time-out */
	while((tick_count < USB_WAIT_PHASE_END) || dfu_activated || !(standalone || master)) {
		if (serviced_tick == QUIET_PHASE_END) {
			init_i2c_line_detect();
			if (!master) {
				usb_teardown();
				usb_disabled = true;
			}
		}

		if (serviced_tick == LINE_DETECT_SAMPLE && !master) {
			uint16_t lines = gpio_get(I2C1_GPIO_PORT, I2C1_GPIO_SCL | I2C1_GPIO_SDA);
			if (lines == I2C1_GPIO_SCL) {
				/* SDA driven low */
				i2c_crossover = 1;
				disp_show(icon_slave);
			} else if (lines == I2C1_GPIO_SCL) {
				/* SCL driven low */
				i2c_crossover = 0;
				disp_show(icon_slave);
			} else {
				/* don't know, assume everyone is dead */
				standalone = true;
			}
		}

		if (serviced_tick == LINE_DETECT_PHASE_END) {
			i2c_init(i2c_crossover);
		}

		if (!usb_disabled)
			usb_run();

		if (!(standalone || master) && serviced_tick > LINE_DETECT_PHASE_END) {
			if (!i2c_poll()) {
				disp_show(icon_flash_problem);
			}
		}

		while (serviced_tick < tick_count) {
			/* per-tick actions */
			serviced_tick++;
			disp_tick();

			/* Blink the LED */
			if (((serviced_tick / 500 % 2) == 0) == 0)
				gpio_set(LED_PORT, LED_PIN);
			else
				gpio_clear(LED_PORT, LED_PIN);
		}
	}

	continue_boot();
}
