#include "i2c.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/crc.h>
#include <libopencm3/cm3/nvic.h>
#include "hw_defs.h"
#include "main.h"
#include "disp.h"
#include "../full/util.h"

#define ERR_MASK (I2C_ISR_ARLO | I2C_ISR_OVR | I2C_ISR_TIMEOUT | I2C_ISR_PECERR \
	| I2C_ISR_TIMEOUT | I2C_ISR_ALERT)

struct i2c_defs {
	uint32_t base;
	enum rcc_periph_clken rcc_clk;
	uint32_t gpio_port;
	enum rcc_periph_clken gpio_rcc_clk;
	uint16_t gpio_scl;
	uint16_t gpio_sda;
	uint8_t gpio_af;
	int irq;
};

static const struct i2c_defs buses[2] = {
{
	.base = I2C1,
	.rcc_clk = RCC_I2C1,
	.gpio_port = I2C1_GPIO_PORT,
	.gpio_rcc_clk = I2C1_GPIO_RCC,
	.gpio_scl = I2C1_GPIO_SCL,
	.gpio_sda = I2C1_GPIO_SDA,
	.gpio_af = GPIO_AF1,
	.irq = NVIC_I2C1_IRQ
},
{
	.base = I2C2,
	.rcc_clk = RCC_I2C2,
	.gpio_port = I2C2_GPIO_PORT,
	.gpio_rcc_clk = I2C2_GPIO_RCC,
	.gpio_scl = I2C2_GPIO_SCL,
	.gpio_sda = I2C2_GPIO_SDA,
	.gpio_af = I2C2_GPIO_AF,
	.irq = NVIC_I2C2_IRQ
},
};

static const struct i2c_defs *bus;
static size_t rx_bytes;
static uint8_t rx_cmd;
static uint8_t *rx_data = page_data;
static union {
	msg_flash flash;
	uint8_t data[10];
} rx_msg;
uint8_t tx_buf[130];
static size_t tx_bytes;
static size_t tx_size;
static bool nack;
static volatile bool tx_running;

static void i2c_handle_byte(uint8_t c)
{
	if (rx_bytes == 0) {
		rx_cmd = c;
		if (rx_cmd != MSG_RELEASE && rx_cmd != MSG_DATA && rx_cmd != MSG_FLASH) {
			nack = true;
			rx_cmd = 0xFF;
		}
	} else {
		if (rx_cmd == MSG_DATA && rx_data < page_data + PAGE_SIZE) {
			*(rx_data++) = c;
		} else if (rx_cmd == MSG_FLASH && rx_bytes < sizeof(msg_flash)) {
			rx_msg.data[rx_bytes] = c;
		} else {
			nack = true;
		}
	}
}

bool i2c_poll(void)
{
	uint32_t b = bus->base;
	uint32_t isr = I2C_ISR(b);
	if (isr & I2C_CR1_ERRIE) {
		return false;
	}

	if (isr & I2C_ISR_TXIS) {
		I2C_TXDR(b) = tx_buf[tx_bytes++];
	}

	if (isr & I2C_ISR_STOPF) {
		I2C_ICR(bus->base) |= I2C_ICR_STOPCF;
		tx_running = false;
	}

	if(isr & I2C_ISR_NACKF) {
		I2C_ICR(bus->base) |= I2C_ICR_NACKCF;
		tx_running = false;
	}

	/* We can block on receive side for a long time, since our only communication
	 * is the i2c bus */
	if (isr & I2C_ISR_ADDR) {
		rx_bytes = 0;
		nack = false;
		I2C_ICR(b) |= I2C_ICR_ADDRCF;

		while (1) {
			while((I2C_ISR(b) & (I2C_ISR_RXNE | I2C_ISR_STOPF)) == 0)
				;

			if (I2C_ISR(b) & I2C_ISR_STOPF)
				break;
			i2c_handle_byte(I2C_RXDR(b));
			rx_bytes++;
			if (nack) {
				I2C_CR2(b) |= I2C_CR2_NACK;
			}
		}

		I2C_ICR(b) |= I2C_ICR_STOPCF;
		if (nack || rx_bytes == 0) {
			__asm__ ("bkpt");
			return false;
		}

		if (rx_cmd == MSG_RELEASE) {
			continue_boot();
		} else if (rx_cmd == MSG_DATA) {
			/* everything done */
		} else if (rx_cmd == MSG_FLASH && rx_bytes == sizeof(msg_flash)) {
			crc_reset();
			if (crc_calculate_block((uint32_t*)page_data, PAGE_SIZE) != rx_msg.flash.crc) {
				__asm__ ("bkpt");
				return false;
			} else {
				rx_data = page_data;
				page_number = rx_msg.flash.page;
				handle_page();
			}
		}
	}
	return true;
}

bool i2c_tx_running(void)
{
	return tx_running;
}

void i2c_send_release(void)
{
	//uint32_t timeout = tick_count + 5;
	tx_buf[0] = MSG_RELEASE;
	i2c_tx_start(1);
	while(i2c_tx_running())
		i2c_poll();
}

void i2c_tx_start(uint8_t len)
{
	tx_bytes = 0;
	tx_running = true;
	i2c_set_7bit_address(bus->base, 0);
	i2c_set_write_transfer_dir(bus->base);
	i2c_set_bytes_to_transfer(bus->base, len);
	i2c_enable_autoend(bus->base);
	i2c_send_start(bus->base);
}

void i2c_init(bool crossover, bool master)
{
	bus = &buses[crossover];
	rcc_set_i2c_clock_hsi(bus->base);
	i2c_reset(bus->base);
	uint8_t pupd = master ? GPIO_PUPD_PULLUP : GPIO_PUPD_NONE;

	gpio_mode_setup(bus->gpio_port, GPIO_MODE_AF, pupd, bus->gpio_scl | bus->gpio_sda);
	gpio_set_af(bus->gpio_port, bus->gpio_af, bus->gpio_scl | bus->gpio_sda);
	gpio_set_output_options(bus->gpio_port, GPIO_OTYPE_OD, GPIO_OSPEED_HIGH, bus->gpio_scl | bus->gpio_sda);

	i2c_peripheral_disable(bus->base);
	//configure ANFOFF DNF[3:0] in CR1
	i2c_enable_analog_filter(bus->base);
	i2c_set_digital_filter(bus->base, 0);
	/* HSI is at 8Mhz */
	i2c_set_speed(bus->base, i2c_speed_fm_400k, 8);
	//configure No-Stretch CR1 (only relevant in slave mode)
	i2c_enable_stretching(bus->base);
	// addressing mode
	i2c_set_7bit_addr_mode(bus->base);
	// have some address, manual says it is needed to activate slave move
	if (!master)
		i2c_set_own_7bit_slave_address(bus->base, 0xF);
	I2C_OAR1(bus->base) |= I2C_OAR1_OA1EN_ENABLE;
	I2C_CR1(bus->base) |= I2C_CR1_GCEN;
	i2c_peripheral_enable(bus->base);
	nvic_enable_irq(bus->irq);
}

void i2c_teardown(void)
{
	i2c_reset(I2C1);
	i2c_reset(I2C2);
	gpio_mode_setup(I2C1_GPIO_PORT,
		GPIO_MODE_INPUT, GPIO_PUPD_NONE, I2C1_GPIO_SCL | I2C1_GPIO_SDA);
	gpio_mode_setup(I2C1_GPIO_PORT,
		GPIO_MODE_INPUT, GPIO_PUPD_NONE, I2C2_GPIO_SCL | I2C2_GPIO_SDA);
}
