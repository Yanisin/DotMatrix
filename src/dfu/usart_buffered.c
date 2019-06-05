#include <stdint.h>
#include <stdbool.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include "hw_defs.h"
#include "../GameOfLife/mgmt_msg.h"
#include "usart_buffered.h"

#define BUF_SIZE 128
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
	msg_header hdr;
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
	struct usart *usart = &usarts[u];
	while (usart_buf_full(&usart->txbuf))
		;

	usart_buf_put(&usart->txbuf, c);
	USART_CR1(usart->reg_base) |= USART_CR1_TXEIE;
}

void usart_init(void)
{
	usart_setup(0);
	usart_setup(1);
	usart_setup(2);
	usart_setup(3);

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
}
