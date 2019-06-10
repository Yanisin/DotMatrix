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
#include <libopencm3/stm32/crc.h>
#include <stdlib.h>

#define DFU_DEBUG


volatile uint32_t tick_count;
static uint32_t serviced_tick;
int page_number;
uint8_t page_data[PAGE_SIZE] __attribute__ ((aligned (4)));
static bool usb_enabled;
static bool i2c_enabled;
static int i2c_crossover;
bool dfu_activated;
enum mode main_mode = MODE_STANDALONE;
static bool flashing;
bool ready_to_flash;
static uint32_t next_page_time;
static int current_i2c_packet = -1;


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

static void send_packet_i2c(void)
{
	tx_buf[0] = MSG_DATA;
	memcpy(tx_buf + 1, page_data + current_i2c_packet * PAGE_PACKET_SIZE, PAGE_PACKET_SIZE);
	i2c_tx_start(PAGE_PACKET_SIZE + 1);
}

static void send_flash_command(void)
{
	msg_flash* msg = (void*)tx_buf;
	crc_reset();
	msg->msgid = MSG_FLASH;
	msg->crc = crc_calculate_block((uint32_t*)page_data, PAGE_SIZE);
	msg->page = page_number;
	i2c_tx_start(sizeof(*msg));
}

void handle_page(void)
{
	uintptr_t page = FLASH_BASE + PAGE_SIZE * (FLASH_RESERVED_PAGES + page_number);
	/* Kick of i2c resend */
	current_i2c_packet = 0;
	ready_to_flash = false;

	if (!flashing) {
		flashing = true;
		disp_show(icon_flash);
	}

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
	if (main_mode == MODE_MASTER)
		gpio_clear(I2C1_GPIO_PORT, I2C1_GPIO_SCL);
}

void continue_boot(void)
{
	if (main_mode == MODE_MASTER)
		i2c_send_release();

	uint32_t wait = tick_count + 5;
	while (wait < tick_count)
		;

	ticker_teardown();
	rcc_periph_reset_pulse(RST_USART1);
	rcc_periph_reset_pulse(RST_USART2);
	rcc_periph_reset_pulse(RST_USART3);
	rcc_periph_reset_pulse(RST_USART4);
	rcc_periph_reset_pulse(RST_USB);
	rcc_periph_reset_pulse(RST_I2C1);
	rcc_periph_reset_pulse(RST_I2C2);
	rcc_periph_reset_pulse(RST_GPIOA);
	rcc_periph_reset_pulse(RST_GPIOB);
	rcc_periph_reset_pulse(RST_GPIOC);
	rcc_periph_reset_pulse(RST_GPIOD);
	rcc_periph_reset_pulse(RST_GPIOE);

	rcc_periph_clock_disable(DISP_GPIOS_RCC);
	rcc_periph_clock_disable(I2C1_GPIO_RCC);
	rcc_periph_clock_disable(I2C2_GPIO_RCC);
	rcc_periph_clock_disable(RCC_USB);
	rcc_periph_clock_disable(RCC_I2C1);
	rcc_periph_clock_disable(RCC_I2C2);
	rcc_periph_clock_disable(RCC_USART1);
	rcc_periph_clock_disable(RCC_USART2);
	rcc_periph_clock_disable(RCC_USART3);
	rcc_periph_clock_disable(RCC_USART4);
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
	ready_to_flash = false;
	usb_enabled = true;
	i2c_enabled = false;

	/* Continue until time-out. If we are a slave or the user has started download,
	 * there is not time-out */
	while((tick_count < USB_WAIT_PHASE_END) || dfu_activated || (main_mode == MODE_SLAVE)) {
		if (serviced_tick == QUIET_PHASE_END) {
			init_i2c_line_detect();
			if (main_mode != MODE_MASTER) {
				rcc_periph_reset_pulse(RST_USB);
				usb_enabled = false;
			}
		}

		if (serviced_tick == LINE_DETECT_SAMPLE && main_mode != MODE_MASTER) {
			uint16_t lines = gpio_get(I2C1_GPIO_PORT, I2C1_GPIO_SCL | I2C1_GPIO_SDA);
			if (lines == I2C1_GPIO_SCL) {
				/* SDA driven low */
				i2c_crossover = 1;
				disp_show(icon_slave);
				main_mode = MODE_SLAVE;
			} else if (lines == I2C1_GPIO_SCL) {
				/* SCL driven low */
				i2c_crossover = 0;
				disp_show(icon_slave);
				main_mode = MODE_SLAVE;
			} else {
				/* don't know, assume everyone is dead */
				main_mode = MODE_STANDALONE;
			}
		}

		if (serviced_tick > LINE_DETECT_SAMPLE && main_mode == MODE_SLAVE && !i2c_enabled) {
			uint16_t lines = gpio_get(I2C1_GPIO_PORT, I2C1_GPIO_SCL | I2C1_GPIO_SDA);
			if (lines == (I2C1_GPIO_SCL | I2C1_GPIO_SDA)) {
				i2c_enabled = true;
				i2c_init(i2c_crossover, false);
			}
		}

		if (serviced_tick == LINE_DETECT_PHASE_END && main_mode == MODE_MASTER) {
			i2c_enabled = true;
			i2c_init(i2c_crossover, true);
		}

		if (main_mode == MODE_MASTER && i2c_enabled && !i2c_tx_running()) {
			if (current_i2c_packet == PAGE_SIZE/PAGE_PACKET_SIZE) {
				current_i2c_packet = -1;
				next_page_time = tick_count + PAGE_DELAY_TIME;
				send_flash_command();
			} else if (current_i2c_packet >= 0){
				send_packet_i2c();
				current_i2c_packet++;
			}
		}

		/* Allow flashing if we do not have any unfinished i2c packets, are in appropriate phase,
		 * and the next_page_time delat has note elapsed */
		if (main_mode == MODE_MASTER
		    && next_page_time < tick_count
		    && current_i2c_packet == -1
		    && serviced_tick > FLASHING_ALLOWED) {
			ready_to_flash = true;
		}

		if (usb_enabled)
			usb_run();

		if (i2c_enabled) {
			if (!i2c_poll()) {
				disp_show(icon_flash_problem);
			}
		}

		if (serviced_tick < tick_count) {
			/* per-tick actions */
			serviced_tick = tick_count;
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
