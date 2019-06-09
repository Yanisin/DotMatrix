#include "i2c.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/cm3/nvic.h>
#include "hw_defs.h"
#include "main.h"
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
static bool nack;

bool i2c_poll(void)
{
	uint32_t b = bus->base;
	uint32_t isr = I2C_ISR(b);
	if (isr & I2C_ISR_ADDR) {
		rx_bytes = 0;
		nack = false;
		I2C_ICR(b) |= I2C_ICR_ADDRCF;
	} else if (isr & I2C_ISR_RXNE) {
		uint8_t c = I2C_RXDR(b);
		if (rx_bytes == 0) {
			rx_cmd = c;
		} else {
			nack = true;
		}
	} else if (isr & I2C_ISR_STOPF) {
		I2C_ICR(b) |= I2C_ICR_STOPCF;
		if (nack)
			return false;
		if (rx_cmd == MSG_RELEASE)
			continue_boot();
	} else if (isr & I2C_CR1_ERRIE) {
		return false;
	}
	return true;
}

bool i2c_tx_wait(uint32_t tick_timeout)
{
	while (!(I2C_ISR(bus->base) & I2C_ISR_STOPF)) {
		if (tick_count > tick_timeout)
			return false;
	}
	I2C_ICR(bus->base) |= I2C_ICR_STOPCF;
	return true;
}

void i2c_send_release(void)
{
	uint32_t timeout = tick_count + 10;
	i2c_tx_start(1);
	i2c_tx_char(MSG_RELEASE, tick_count);
	i2c_tx_wait(tick_count);
	while (tick_count < timeout)
		;
}

bool i2c_tx_char(uint8_t c, uint32_t tick_timeout)
{
	while (i2c_transmit_int_status(bus->base)) {
		if (tick_count > tick_timeout)
			return false;
	}
	i2c_send_data(bus->base, c);
	return true;
}

bool i2c_tx_data(uint8_t size, const uint8_t* data,  uint32_t tick_timeout)
{
	for(uint8_t i = 0; i < size; i++)
		if(!i2c_tx_char(data[i], tick_timeout))
			return false;
	return true;
}

void i2c_tx_start(uint8_t len)
{
	i2c_set_7bit_address(bus->base, 0);
	i2c_set_write_transfer_dir(bus->base);
	i2c_set_bytes_to_transfer(bus->base, len);
	i2c_enable_autoend(bus->base);
	i2c_send_start(bus->base);
}

void i2c_init(bool crossover)
{
	bus = &buses[crossover];
	rcc_periph_clock_enable(bus->rcc_clk);
	rcc_periph_clock_enable(bus->gpio_rcc_clk);
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
	i2c_set_speed(bus->base, i2c_speed_sm_100k, 8);
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
