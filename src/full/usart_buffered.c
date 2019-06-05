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
#include "applet.h"
#include "console.h"
#include "pollint.h"

/* ---------------- Macro Definition --------------- */

#ifdef SIM
/* Work-around for lack of rate limiting */
#define BUF_SIZE 256
#else
#define BUF_SIZE 32
#endif
#define USART_COUNT 4

struct usart_buffer {
	uint8_t buf[BUF_SIZE];
	volatile uint32_t wp;
	volatile uint32_t rp;
};

/* ---------------- Local Variables --------------- */

static struct usart {
	uint32_t reg_base;
	struct usart_buffer rxbuf;
	struct usart_buffer txbuf;
	bool header_received;
	usart_header hdr;
} usarts[USART_COUNT];

/* ---------------- Local Functions --------------- */

static int usart_buf_empty(struct usart_buffer *buf)
{
	return (buf->wp - buf->rp) == 0;
}

static int usart_buf_full(struct usart_buffer *buf)
{
	return (buf->wp - buf->rp) >= BUF_SIZE-1;
}

static int usart_buf_available(struct usart_buffer *buf)
{
	return buf->wp - buf->rp;
}

static void usart_buf_put(struct usart_buffer *buf, uint8_t val)
{
	if (!usart_buf_full(buf)) {
		buf->buf[buf->wp % BUF_SIZE] = val;
		++buf->wp;
	}
}

static uint32_t usart_buf_get(struct usart_buffer *buf)
{
	uint32_t val = -1;
	if (!usart_buf_empty(buf)) {
		val = buf->buf[buf->rp % BUF_SIZE];
		++buf->rp;
	}
	return val;
}

static void usart_setup(uint8_t usart)
{
	usarts[usart].rxbuf.wp = 0;
	usarts[usart].rxbuf.rp = 0;
	usarts[usart].txbuf.wp = 0;
	usarts[usart].txbuf.rp = 0;
	usarts[usart].hdr.usart = usart;
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
	uint8_t c;

	if (((USART_CR1(usart->reg_base) & USART_CR1_RXNEIE) != 0) &&
			((USART_ISR(usart->reg_base) & USART_ISR_RXNE) != 0)) {

		/* Read one byte from the receive data register */
		c = usart_recv(usart->reg_base);
		/* try putting it into the rx buffer, drop if buffer full */
		usart_buf_put(&usart->rxbuf, c);
	}

	if (((USART_CR1(usart->reg_base) & USART_CR1_TXEIE) != 0) &&
			((USART_ISR(usart->reg_base) & USART_ISR_TXE) != 0)) {

		/*check if there is something in the queue to transmit */
		if (!usart_buf_empty(&usart->txbuf)) {
			c = usart_buf_get(&usart->txbuf);
			/* Write one byte to the transmit data register */
			usart_send(usart->reg_base, c);
		} else {
			/* queue empty */
			/* Disable the USARTy Transmit interrupt */
			USART_CR1(usart->reg_base) &= ~USART_CR1_TXEIE;
		}
	}
}

void usart1_isr(void)
{
	usart_isr_common(0);
}


void usart2_isr(void)
{
	usart_isr_common(1);
}

void usart3_4_isr(void)
{
	usart_isr_common(2);
	usart_isr_common(3);
}
#endif

/* ---------------- Global Functions --------------- */

void usart_send_msg(uint8_t usart, uint8_t id, uint8_t len, const void *data)
{
	usart_send_char(usart, len);
	usart_send_char(usart, id);
	for(uint8_t i = 0; i < len; i++) {
		uint8_t c = ((const uint8_t*)data)[i];
		usart_send_char(usart, c);
	}
}

void usart_send_char(uint8_t u, uint8_t c)
{
#ifndef SIM
	struct usart *usart = &usarts[u];
	while (usart_buf_full(&usart->txbuf))
		;

	usart_buf_put(&usart->txbuf, c);
	USART_CR1(usart->reg_base) |= USART_CR1_TXEIE;
#else
	uint8_t buf[2] = {u, c};
	sim_send(MSGID_UART_TX, 2, &buf);
#endif
}

#ifdef SIM
#include <stdio.h>
#include <stdlib.h>
void usart_sim_recv(uint8_t u, const uint8_t *data, size_t len)
{
	struct usart *usart = &usarts[u];
	for(size_t i = 0; i < len; i++) {
		if (usart_buf_full(&usart->rxbuf)) {
			printf("USART buffer overrun\n");
			abort();
		}
		usart_buf_put(&usart->rxbuf, data[i]);
	}
}
#endif

const usart_header* usart_peek(uint8_t u)
{
	// TODO: add some self-synchronization to the protocol,
	// like with the old game of life protocol
	struct usart *usart = &usarts[u];
	usart_header *hdr = &usart->hdr;
	if (!usart->header_received && usart_buf_available(&usart->rxbuf) >= 2) {
		usart->header_received = true;
		hdr->length = usart_buf_get(&usart->rxbuf);
		hdr->id = usart_buf_get(&usart->rxbuf);
		hdr->remaining = hdr->length;
	}

	if (usart->header_received) {
		/* Update the number of available bytes */
		int avail = usart_buf_available(&usart->rxbuf);
		if (avail > hdr->length) {
			avail = hdr->length;
		}
		hdr->available = avail;
		return &usart->hdr;
	} else {
		return NULL;
	}
}

uint8_t usart_recv_partial(uint8_t u, size_t size,  void *buf)
{
	struct usart *usart = &usarts[u];
	uint8_t recv = usart->hdr.available;
	if (size < recv) {
		recv = size;
	}

	for(uint8_t i = 0; i < recv; i++) {
		((uint8_t*)buf)[i] = usart_buf_get(&usart->rxbuf);
	}
	usart->hdr.available = 0;
	usart->hdr.remaining -= recv;
	if (usart->hdr.remaining == 0) {
		usart->header_received = false;
	}
	return recv;
}

bool usart_recv_msg(uint8_t u, size_t size, void *buf)
{
	struct usart *usart = &usarts[u];
	uint8_t recv;
	if (!usart_is_complete(&usart->hdr)) {
		return false;
	}
	if (usart->hdr.length > size) {
		usart_skip(u);
		return false;
	}
	recv = usart_recv_partial(u, size, buf);
	assert(recv == usart->hdr.length);
	return true;

}

void usart_skip(uint8_t u)
{
	struct usart *usart = &usarts[u];
	while(usart->hdr.available) {
		usart_buf_get(&usart->rxbuf);
		usart->hdr.available --;
		usart->hdr.remaining--;
	}
	if (usart->hdr.remaining == 0) {
		usart->header_received = false;
	}
}

void usart_recv_dispatch(usart_msg_dispatcher dispatch)
{
	for(int u = 0; u < USART_COUNT; u++) {
		const usart_header *hdr;
		while((hdr = usart_peek(u)) != NULL) {
			bool recognized = dispatch(hdr);
			if (!recognized) {
				usart_skip(u);
			}
			poll_int();
		}
	}
}

void usart_buf_clear(int u)
{
	struct usart *usart = &usarts[u];
	usart->header_received = false;
	chSysLock();
	usart->rxbuf.rp = usart->rxbuf.wp = 0;
	usart->txbuf.rp = usart->txbuf.wp = 0;
	chSysUnlock();
}

void usart_init(void)
{
	usart_setup(0);
	usart_setup(1);
	usart_setup(2);
	usart_setup(3);

#ifndef SIM
	usart_setup_hw(0,
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

	usart_setup_hw(1,
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

	usart_setup_hw(2,
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

	usart_setup_hw(3,
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
