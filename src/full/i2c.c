#include "i2c.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/cm3/nvic.h>
#include <string.h>
#include "led.h"
#include "topo.h"
#include "util.h"
#include "console.h"
#include "hw_defs.h"
#include "byte_queue.h"
#include <assert.h>

/* Does not need to hold the whole message */
#define TX_BUF_SIZE 8

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

struct i2c_msg_header {
	/* Len is sent beforehand so that we can allocate place in queue */
	uint8_t len;
	uint8_t id;
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


static bool rx_active;
static const struct i2c_defs *bus;
static msg_header parsed_msg;
static buf_ptr rx_buf;
static msg_rx_queue *rx_queue;
static bool rx_skip_message;
union {
	uint8_t buf[sizeof(struct i2c_msg_header)];
	struct i2c_msg_header msg;
} rx;
static uint32_t crc_state;
static uint8_t rx_bytes;


static uint8_t tx_buf[TX_BUF_SIZE];
/* Queue is reset if an error is detected.
 * These ISRs can be set:
 *   * ARLO: arbitration lost, retry sending
 *   * NACKF, any other error: cancel
 */
static output_queue_t tx_queue;
/* Only one thread can try to send data at a time */
MUTEX_DECL(tx_mutex);
BSEMAPHORE_DECL(tx_finished_sem, true);
/* Is there anyone servicing the transfer now? If no, the ISR will try to cancel it. */
static bool tx_active;
static uint32_t tx_isr;

static inline uint8_t cell_to_i2c(uint8_t i)
{
	return i + 1;
}

static inline uint8_t i2c_to_cell(uint8_t i)
{
	return i - 1;
}

static void rx_complete(void)
{
	chSysLockFromISR();
	if (!rx_skip_message) {
		msg_rx_queue_commitI(rx_queue, &rx_buf);
	} else {
		msg_rx_queue_rejectI(rx_queue, &rx_buf);
	}
	chSysUnlockFromISR();
}

static void i2c_header_buffered(void)
{
	parsed_msg.source = MSG_SOURCE_I2C;
	parsed_msg.flags = 0;
	parsed_msg.length = rx.msg.len;
	parsed_msg.id = rx.msg.id & MSG_FULL_ID_MASK;
	rx_queue = msg_dispatcher(&parsed_msg);
	if (rx_queue) {
		bool reserved;
		chSysLockFromISR();
		reserved = msg_rx_queue_reserveI(rx_queue, &parsed_msg, &rx_buf);
		chSysUnlockFromISR();
		if (!reserved) {
#ifdef DEBUG
			chSysHalt("i2c too long overrun\n");
#endif
			console_printf("i2c Message skipped -- no space\n");
			rx_skip_message = true;
		}
	} else {
		rx_skip_message = true;
	}
}

static void rx_byte(uint8_t c)
{
	rx_bytes++;
	if (rx_bytes <= sizeof(struct i2c_msg_header)) {
		/* header */
		crc8(&crc_state, c);
		rx.buf[rx_bytes - 1] = c;
		if (rx_bytes == sizeof(struct i2c_msg_header)) {
			i2c_header_buffered();
		}
	} else if (rx_bytes <= sizeof(struct i2c_msg_header) + parsed_msg.length) {
		/* data */
		if (rx_skip_message)
			return;
		crc8(&crc_state, c);
		*buf_ptr_index(&rx_buf, rx_bytes - 1 - sizeof(struct i2c_msg_header)) = c;
	} else if (rx_bytes == sizeof (struct i2c_msg_header) + parsed_msg.length + 1) {
		/* crc */
		if (crc8_get(&crc_state) != c)
			rx_skip_message = true;
	} else {
		rx_skip_message = true;
	}
}

#define ERR_MASK (I2C_ISR_ARLO | I2C_ISR_OVR | I2C_ISR_TIMEOUT | I2C_ISR_PECERR \
	| I2C_ISR_TIMEOUT | I2C_ISR_ALERT)

static void i2c_isr(void)
{
	uint32_t b = bus->base;
	uint32_t isr = I2C_ISR(b);
	if (isr & I2C_ISR_ADDR) {
		assert(!rx_active);
		rx_bytes = 0;
		rx_skip_message = false;
		crc_state = 0;
		rx_active = true;
		I2C_ICR(b) |= I2C_ICR_ADDRCF;
	} else if (isr & I2C_ISR_TXIS) {
		/* We currently do not support i2c reads */
		assert(!rx_active);
		if (tx_active) {
			msg_t qchar;
			tx_isr |= I2C_ISR_TXIS;
			chSysLockFromISR();
			qchar = oqGetI(&tx_queue);
			if (qchar == MSG_TIMEOUT) {
				i2c_disable_interrupt(b, I2C_ISR_TXIS);
			} else {
				I2C_TXDR(b) = qchar;
			}
			chSysUnlockFromISR();
		} else {
			/* Just try to end the transfer */
			I2C_CR2(b) |= I2C_CR2_STOP;
			I2C_TXDR(b) = 0xFF;
		}
	} else if (isr & I2C_ISR_RXNE) {
		assert(rx_active);
		/* Note: we do not nack messages currently, we would need to setup NBYTES and
		 * the functionality is not yet needed by anyone. And it is not usable with global
		 * address anyway */
		rx_byte(I2C_RXDR(b));
	} else if (isr & I2C_ISR_STOPF) {
		if (rx_active) {
			rx_active = false;
			rx_complete();
		} else if (tx_active) {
			/* We should not get STOPF, but report it anyway */
			chSysLockFromISR();
			tx_isr |= I2C_ISR_STOPF;
			oqResetI(&tx_queue);
			chBSemSignalI(&tx_finished_sem);
			chSysUnlockFromISR();
		}
		I2C_ICR(b) |= I2C_ICR_STOPCF;
	} else if (isr & I2C_ISR_NACKF) {
		if (tx_active) {
			chSysLockFromISR();
			tx_isr |= I2C_ISR_NACKF;
			oqResetI(&tx_queue);
			chBSemSignalI(&tx_finished_sem);
			chSysUnlockFromISR();
		}
		I2C_ICR(b) |= I2C_ICR_NACKCF;
	} else if (isr & ERR_MASK) {
		if (rx_active) {
			rx_skip_message = true;
		} else if (tx_active){
			chSysLockFromISR();
			tx_isr |= isr & ERR_MASK;
			oqResetI(&tx_queue);
			chBSemSignalI(&tx_finished_sem);
			chSysUnlockFromISR();
		}
		I2C_ICR(b) |= isr & ERR_MASK;
	}
}

void i2c1_isr(void) {
	CH_IRQ_PROLOGUE();
	i2c_isr();
	CH_IRQ_EPILOGUE();
}

void i2c2_isr(void) {
	CH_IRQ_PROLOGUE();
	i2c_isr();
	CH_IRQ_EPILOGUE();
}

static void i2c_notify_send(io_queue_t *p)
{
	(void)p;
	i2c_enable_interrupt(bus->base, I2C_CR1_TXIE);
}

static void i2c_send_byte(uint8_t byte)
{
	/* Note: the locking can maybe be eliminated if we are ok with garbage in the queue ? */
	chSysLock();
	if (tx_isr == 0)
		oqPutI(&tx_queue, byte);
	chSysUnlock();
}

static bool i2c_send_raw(uint8_t i2c_addr, uint8_t msgid, uint8_t len, const void *gdata)
{
	size_t total = sizeof(struct i2c_msg_header) + len +1;
	assert(total < 255);
	const uint8_t *data = gdata;
	struct i2c_msg_header hdr;
	hdr.id = msgid;
	hdr.len = len;

	uint32_t crc = 0;
	crc8(&crc, hdr.len);
	crc8(&crc, hdr.id);
	for (uint8_t i = 0; i < len; i++) {
		crc8(&crc, data[i]);
	}

	while(1) {
		i2c_set_7bit_address(bus->base, i2c_addr);
		i2c_set_write_transfer_dir(bus->base);
		i2c_set_bytes_to_transfer(bus->base, total);
		i2c_enable_autoend(bus->base);

		chSysLock();
		oqResetI(&tx_queue);
		tx_active = true;
		tx_isr = 0;
		i2c_send_start(bus->base);
		chSysUnlock();

		i2c_send_byte(hdr.len);
		i2c_send_byte(hdr.id);
		for (uint8_t i = 0; i < len; i++) {
			i2c_send_byte(data[i]);
		}
		i2c_send_byte(crc8_get(&crc));
		chBSemWait(&tx_finished_sem);

		chSysLock();
		tx_active = false;
		if (tx_isr & I2C_ISR_ARLO) {
			chSysUnlock();
			continue;
		} else if (tx_isr & I2C_ISR_STOPF) {
			chSysUnlock();
			return true;
		} else {
			chSysUnlock();
			return false;
		}
		chSysUnlock();
	}

}

bool i2c_send(uint8_t cell, uint8_t msgid, uint8_t len, const void *data)
{
	if (cell == topo_my_id)
		return false;
	chMtxLock(&tx_mutex);
	bool r = i2c_send_raw(cell_to_i2c(cell), msgid, len, data);
	chMtxUnlock(&tx_mutex);
	return r;
}

bool i2c_broadcast(uint8_t msgid, uint8_t len, const void *data)
{
	chMtxLock(&tx_mutex);
	bool r = i2c_send_raw(0, msgid, len, data);
	chMtxUnlock(&tx_mutex);
	return r;
}

void i2c_init(void)
{
	bus = &buses[ABS(topo_master_position.x + topo_master_position.y) % 2];
	oqObjectInit(&tx_queue, tx_buf, sizeof(tx_buf), i2c_notify_send, NULL);

	rcc_periph_clock_enable(bus->rcc_clk);
	rcc_periph_clock_enable(bus->gpio_rcc_clk);
	rcc_set_i2c_clock_hsi(bus->base);
	i2c_reset(bus->base);

	gpio_mode_setup(bus->gpio_port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, bus->gpio_scl | bus->gpio_sda);
	gpio_set_af(bus->gpio_port, bus->gpio_af, bus->gpio_scl | bus->gpio_sda);
	gpio_set_output_options(bus->gpio_port, GPIO_OTYPE_OD, GPIO_OSPEED_HIGH, bus->gpio_scl | bus->gpio_sda);

	i2c_peripheral_disable(bus->base);
	//configure ANFOFF DNF[3:0] in CR1
	i2c_enable_analog_filter(bus->base
				 );
	i2c_set_digital_filter(bus->base, 0);
	/* HSI is at 8Mhz */
	i2c_set_speed(bus->base, i2c_speed_sm_100k, 8);
	//configure No-Stretch CR1 (only relevant in slave mode)
	if (topo_is_master)
		i2c_enable_stretching(bus->base);
	else
		i2c_disable_stretching(bus->base);
	//addressing mode
	i2c_set_7bit_addr_mode(bus->base);
	i2c_set_own_7bit_slave_address(bus->base, cell_to_i2c(topo_my_id));
	I2C_OAR1(bus->base) |= I2C_OAR1_OA1EN_ENABLE;
	I2C_CR1(bus->base) |= I2C_CR1_GCEN;
	i2c_enable_interrupt(bus->base, I2C_CR1_ADDRIE | I2C_CR1_RXIE |
		I2C_CR1_STOPIE | I2C_CR1_NACKIE | I2C_CR1_ERRIE);
	i2c_peripheral_enable(bus->base);
	nvic_enable_irq(bus->irq);
}
