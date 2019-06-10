/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SIM
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#else
#include "sim/sim.h"
#include "sim/proto_defs.h"
#endif
#include <assert.h>
#include <ch.h>
#include "hw_defs.h"
#include "usart_buffered.h"
#include "console.h"
#include "byte_queue.h"
#include "applet.h"
#include "console.h"
#include "util.h"

/* ---------------- Macro Definition --------------- */
#define USART_COUNT 4
#define TX_BUF_SIZE 16
#define USART_ROUTE_SHIFT 6
#define USART_ROUTE_MASK (0x3 << USART_ROUTE_SHIFT)
#define USART_RTOR_RESYNC TIME_MS2I(5)

typedef struct usart_msg {
	uint8_t len;
	uint8_t id_flags;
} usart_msg;

/* ---------------- Local Variables --------------- */

static struct usart {
	uint32_t reg_base;
	/* using the usart directions */
	uint8_t num;

	buf_ptr rx_buf;
	msg_rx_queue *rx_queue;
	bool skip_message;
	msg_header parsed_msg;
	union {
		uint8_t rx_header_buf[sizeof(usart_msg)];
		usart_msg rx_msg;
	};
	uint32_t crc_state;
	uint8_t rx_bytes;
	bool rx_garbage;

	virtual_timer_t tx_timeout;
	mutex_t tx_mutex;
	output_queue_t tx_queue;
	uint8_t tx_buf[TX_BUF_SIZE];
} usarts[USART_COUNT];

/* ---------------- Global Variables --------------- */

/* ---------------- Local Functions --------------- */

#ifndef SIM
static enum direction usart_to_direction(uint8_t usart);
#endif

static void usart_resync(void *usartg)
{
	struct usart *usart = usartg;
	/* There is small race condition where USART can be reactivated during the
	 * resync. FIXME */
	usart->rx_garbage = false;
}

static void usart_problemI(struct usart *usart)
{
	usart->rx_garbage = true;
	/* We can not use RTOR since not all the USARts have it */
	chVTSetI(&usart->tx_timeout, USART_RTOR_RESYNC, usart_resync, usart);
}

static void usart_header_buffered(struct usart *usart)
{
	usart->parsed_msg.source = usart->num;
	usart->parsed_msg.flags = (usart->rx_msg.id_flags & USART_ROUTE_MASK) >> USART_ROUTE_SHIFT;
	usart->parsed_msg.length = usart->rx_msg.len;
	usart->parsed_msg.id = usart->rx_msg.id_flags & MSG_FULL_ID_MASK;
	usart->rx_queue = msg_dispatcher(&usart->parsed_msg);
	if (usart->rx_queue) {
		bool reserved;
		chSysLockFromISR();
		reserved = msg_rx_queue_reserveI(usart->rx_queue, &usart->parsed_msg, &usart->rx_buf);
		chSysUnlockFromISR();
		if (reserved) {
			usart->skip_message = false;
		} else {
#ifdef DEBUG
			chSysHalt("uart overrun\n");
#endif
			console_printf("Message skipped -- no space\n");
			usart->skip_message = true;
		}
	} else {
		usart->skip_message = true;
	}
}

static void usart_start_new_msg(struct usart *usart)
{
	usart->crc_state = 0;
	usart->rx_bytes = 0;
}

/* Main state machine parsing the messages */
static void usart_handle_char(struct usart *usart, uint8_t c)
{
	usart->rx_bytes++;
	if (usart->rx_bytes <= sizeof(usart_msg)) {
		/* get header */
		crc8(&usart->crc_state, c);
		usart->rx_header_buf[usart->rx_bytes - 1] = c;
		if (usart->rx_bytes == sizeof(usart_msg)) {
			usart_header_buffered(usart);
		}
	} else if (usart->rx_bytes <= sizeof(usart_msg) + usart->parsed_msg.length) {
		/* get data */
		if (usart->skip_message)
			return;
		crc8(&usart->crc_state, c);
		*buf_ptr_index(&usart->rx_buf, usart->rx_bytes - 1 - sizeof(usart_msg)) = c;
	} else if (usart->rx_bytes == sizeof (usart_msg) + usart->parsed_msg.length + 1) {
		/* check CRC and finish */
		bool crc_ok = crc8_get(&usart->crc_state) == c;
		if (usart->skip_message) {
			usart_start_new_msg(usart);
			return;
		}
		chSysLockFromISR();
		if (crc_ok) {
			msg_rx_queue_commitI(usart->rx_queue, &usart->rx_buf);
		} else {
			msg_rx_queue_rejectI(usart->rx_queue, &usart->rx_buf);
			usart_problemI(usart);
		}
		chSysUnlockFromISR();
		usart_start_new_msg(usart);
	}
}

static void usart_kick_sendI(io_queue_t *iq)
{
	struct usart *usart = iq->q_link;
#ifndef SIM
	usart_enable_tx_interrupt(usart->reg_base);
#else
	uint8_t buf[2] = {usart->num, oqGetI(&usart->tx_queue)};
	sim_send(MSGID_UART_TX, 2, &buf);
#endif
}

static void usart_setup(uint8_t u)
{
	struct usart *usart = &usarts[u];
	usart->rx_bytes = 0;
	usart->crc_state = 0;
	usart->num = u;
	chMtxObjectInit(&usart->tx_mutex);
	oqObjectInit(&usart->tx_queue, usart->tx_buf, sizeof(usart->tx_buf), usart_kick_sendI, usart);
}

#ifndef SIM
static void usart_setup_hw(
		uint8_t usart_num,
		uint32_t usart, 
		enum rcc_periph_clken usart_rcc,
		uint8_t irq, 
		enum rcc_periph_clken rx_rcc,
		uint32_t rx_port,
		uint16_t rx_pin,
		uint8_t rx_af_num,
		enum rcc_periph_clken tx_rcc,
		uint32_t tx_port,
		uint16_t tx_pin,
		uint8_t tx_af_num
		)
{
	usarts[usart_num].reg_base = usart;
	rcc_periph_clock_enable(usart_rcc);
	rcc_periph_clock_enable(rx_rcc);
	rcc_periph_clock_enable(tx_rcc);
	nvic_enable_irq(irq);

	gpio_mode_setup(tx_port, GPIO_MODE_AF, GPIO_PUPD_NONE, tx_pin);
	gpio_set_af(tx_port, tx_af_num, tx_pin);

	gpio_mode_setup(rx_port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, rx_pin);
	gpio_set_af(rx_port, rx_af_num, rx_pin);

	/* Setup UART parameters. */
	usart_set_baudrate(usart, USART_BAUDRATE);
	usart_set_databits(usart, 8);
	usart_set_stopbits(usart, USART_STOPBITS_1);
	usart_set_parity(usart, USART_PARITY_NONE);
	usart_set_flow_control(usart, USART_FLOWCONTROL_NONE);
	usart_set_mode(usart, USART_MODE_TX_RX);

	/* Enable USART Receive interrupt. */
	USART_CR1(usart) |= USART_CR1_RXNEIE;

	/* Finally enable the USART. */
	usart_enable(usart);
}

/* ---------------- Interrupt Routines --------------- */

static void usart_isr_common(int usart_idx)
{
	struct usart *usart = &usarts[usart_idx];
	uint32_t isr = USART_ISR(usart->reg_base);

	if (isr & USART_ISR_ORE) {
		chSysLockFromISR();
		usart_problemI(usart);
		chSysUnlockFromISR();
		USART_ICR(usart->reg_base) |= USART_ICR_ORECF;
	}

	if (isr & USART_ISR_RXNE) {
		/* Read one byte from the receive data register */
		uint8_t c = usart_recv(usart->reg_base);
		if (!usart->rx_garbage) {
			usart_handle_char(usart, c);
		} else {
			chSysLockFromISR();
			usart_problemI(usart);
			chSysUnlockFromISR();
		}
	}

	if (isr & USART_ISR_TXE) {
		msg_t qchar;
		chSysLockFromISR();
		qchar = oqGetI(&usart->tx_queue);
		chSysUnlockFromISR();
		if (qchar == MSG_TIMEOUT) {
			/* queue empty */
			/* Disable the USART Transmit interrupt */
			USART_CR1(usart->reg_base) &= ~USART_CR1_TXEIE;
		} else {
			usart_send(usart->reg_base, qchar);
		}
	}
}

void usart1_isr(void)
{
	CH_IRQ_PROLOGUE();
	usart_isr_common(usart_to_direction(0));
	CH_IRQ_EPILOGUE();
}


void usart2_isr(void)
{
	CH_IRQ_PROLOGUE();
	usart_isr_common(usart_to_direction(1));
	CH_IRQ_EPILOGUE();
}

void usart3_4_isr(void)
{
	CH_IRQ_PROLOGUE();
	usart_isr_common(usart_to_direction(2));
	usart_isr_common(usart_to_direction(3));
	CH_IRQ_EPILOGUE();
}
#endif
/* ---------------- Global Functions --------------- */

static void usart_send_msg_header(struct usart *usart, uint8_t id, uint8_t flags, uint8_t length, uint32_t *crc)
{
	assert((id & MSG_FULL_ID_MASK) == id);
	uint8_t route = (flags & MSG_ROUTE_MASK) >> MSG_ROUTE_SHIFT;
	uint8_t id_flags = (id & MSG_FULL_ID_MASK) | (route << USART_ROUTE_SHIFT);
	oqPut(&usart->tx_queue, length);
	crc8(crc, length);
	oqPut(&usart->tx_queue, id_flags);
	crc8(crc, id_flags);
}

void usart_send_msg(uint8_t u, uint8_t id, uint8_t flags, uint8_t length, const void *data)
{
	struct usart *usart = &usarts[u];
	uint32_t crc = 0;
	chMtxLock(&usart->tx_mutex);
	usart_send_msg_header(usart, id, flags, length, &crc);
	for (uint8_t i = 0; i < length; i++)
	{
		uint8_t c = ((const uint8_t*)data)[i];
		oqPut(&usart->tx_queue, c);
		crc8(&crc, c);
	}
	oqPut(&usart->tx_queue, crc8_get(&crc));
	chMtxUnlock(&usart->tx_mutex);
}

void usart_send_msg_buf(uint8_t u, uint8_t id, uint8_t flags, uint8_t length, const buf_ptr *src)
{
	struct usart *usart = &usarts[u];
	uint32_t crc = 0;
	chMtxLock(&usart->tx_mutex);
	usart_send_msg_header(usart, id, flags, length, &crc);
	for (uint8_t i = 0; i < length; i++)
	{
		uint8_t c = *buf_ptr_index(src, i);
		oqPut(&usart->tx_queue, c);
		crc8(&crc, c);
	}
	oqPut(&usart->tx_queue, crc8_get(&crc));
	chMtxUnlock(&usart->tx_mutex);
}

#ifdef SIM
void usart_sim_recv(uint8_t u, const uint8_t *data, size_t len)
{
	struct usart *usart = &usarts[u];
	for(size_t i = 0; i < len; i++) {
		usart_handle_char(usart, data[i]);
	}
}
#endif

#ifndef SIM
static enum direction usart_to_direction(uint8_t usart)
{
	switch (usart) {
	case USART_DIR_UP:
		return DIR_UP;
	case USART_DIR_DOWN:
		return DIR_DOWN;
	case USART_DIR_LEFT:
		return DIR_LEFT;
	case USART_DIR_RIGHT:
		return DIR_RIGHT;
	default:
		return DIR_COUNT;
	}
}
#endif

static void usart_init(void)
{
	usart_setup(0);
	usart_setup(1);
	usart_setup(2);
	usart_setup(3);

#ifndef SIM
	usart_setup_hw(usart_to_direction(0),
			USART_A_REG,
			USART_A_RCC,
			USART_A_IRQ,
			USART_A_RX_RCC,
			USART_A_RX_PORT,
			USART_A_RX_PIN,
			USART_A_RX_AF_NUM,
			USART_A_TX_RCC,
			USART_A_TX_PORT,
			USART_A_TX_PIN,
			USART_A_TX_AF_NUM);

	usart_setup_hw(usart_to_direction(1),
			USART_B_REG,
			USART_B_RCC,
			USART_B_IRQ,
			USART_B_RX_RCC,
			USART_B_RX_PORT,
			USART_B_RX_PIN,
			USART_B_RX_AF_NUM,
			USART_B_TX_RCC,
			USART_B_TX_PORT,
			USART_B_TX_PIN,
			USART_B_TX_AF_NUM);

	usart_setup_hw(usart_to_direction(2),
			USART_C_REG,
			USART_C_RCC,
			USART_C_IRQ,
			USART_C_RX_RCC,
			USART_C_RX_PORT,
			USART_C_RX_PIN,
			USART_C_RX_AF_NUM,
			USART_C_TX_RCC,
			USART_C_TX_PORT,
			USART_C_TX_PIN,
			USART_C_TX_AF_NUM);

	usart_setup_hw(usart_to_direction(3),
			USART_D_REG,
			USART_D_RCC,
			USART_D_IRQ,
			USART_D_RX_RCC,
			USART_D_RX_PORT,
			USART_D_RX_PIN,
			USART_D_RX_AF_NUM,
			USART_D_TX_RCC,
			USART_D_TX_PORT,
			USART_D_TX_PIN,
			USART_D_TX_AF_NUM);
#endif
}
init_add(usart_init);
