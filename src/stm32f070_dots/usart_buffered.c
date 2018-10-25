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

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>

#include "usart_buffered.h"



/* ---------------- Macro Definition --------------- */

#define TX_BUF_SIZE 20
#define RX_BUF_SIZE 20

/* ring buffer macros */
#define GET_NEXT(buf_size,pos) ((pos) == ((buf_size)-1)?0:(pos)+1)
#define IS_NOT_EMPTY(head,tail)  ((head) != (tail))
#define IS_NOT_FULL(buf_size,head,tail)  (((head) == ((buf_size)-1)?0:(head)+1)!=(tail))

/* ---------------- Local Variables --------------- */

static volatile uint8_t tx_buf_head[4];
static volatile uint8_t tx_buf_tail[4];
static volatile uint8_t rx_buf_head[4];
static volatile uint8_t rx_buf_tail[4];

static volatile uint8_t tx_buf[4][TX_BUF_SIZE]; 
static volatile uint8_t rx_buf[4][RX_BUF_SIZE];

/* ---------------- Local Functions --------------- */

static void usart_setup(
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
	rcc_periph_clock_enable(usart_rcc);
	rcc_periph_clock_enable(rx_rcc);
	rcc_periph_clock_enable(tx_rcc);
	/* Enable the USART2 interrupt. */
	//nvic_enable_irq(NVIC_USART3_4_IRQ);
	nvic_enable_irq(irq);

	/* Setup GPIO pin GPIO_USART2_RE_TX on GPIO port A for transmit. */
	/* Setup GPIO pin GPIO_USART2_RE_RX on GPIO port A for receive. */
//	gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
//		      GPIO_CNF_INPUT_FLOAT, GPIO_USART2_RX);

	gpio_mode_setup(tx_port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
			tx_pin);
	gpio_mode_setup(tx_port, GPIO_MODE_AF, GPIO_PUPD_NONE, tx_pin);
	gpio_set_af(tx_port, tx_af_num, tx_pin);

	gpio_mode_setup(rx_port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
			rx_pin);
	gpio_mode_setup(rx_port, GPIO_MODE_AF, GPIO_PUPD_NONE, rx_pin);
	gpio_set_af(rx_port, rx_af_num, rx_pin);



	/* Setup UART parameters. */
	usart_set_baudrate(usart, USART_BAUDRATE);
	usart_set_databits(usart, 8);
	usart_set_stopbits(usart, USART_STOPBITS_1);
	usart_set_parity(usart, USART_PARITY_NONE);
	usart_set_flow_control(usart, USART_FLOWCONTROL_NONE);
	usart_set_mode(usart, USART_MODE_TX_RX);

	/* Enable USART2 Receive interrupt. */
	USART_CR1(usart) |= USART_CR1_RXNEIE;

	/* Finally enable the USART. */
	usart_enable(usart);
}

/* ---------------- Interrupt Routines --------------- */

void usart1_isr(void)
{
	uint8_t c;

	if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
			((USART_ISR(USART1) & USART_ISR_RXNE) != 0)) {
		gpio_toggle(GPIOF, GPIO0);
		c= usart_recv(USART1);
		/* Read one byte from the receive data register */
		/*FIXME optimize this*/
		if(IS_NOT_FULL(RX_BUF_SIZE, rx_buf_head[0], rx_buf_tail[0])) {
			rx_buf[0][rx_buf_head[0]]=c;
			rx_buf_head[0] = GET_NEXT(RX_BUF_SIZE, rx_buf_head[0]);
		}

	}

	if (((USART_CR1(USART1) & USART_CR1_TXEIE) != 0) &&
			((USART_ISR(USART1) & USART_ISR_TXE) != 0)) {
		/*check if there is something in the queue to transmit */

		if(IS_NOT_EMPTY(tx_buf_head[0],tx_buf_tail[0]))
		{
			c = tx_buf[0][tx_buf_tail[0]];
			tx_buf_tail[0] = GET_NEXT(TX_BUF_SIZE,tx_buf_tail[0]);

			/* Write one byte to the transmit data register */
			usart_send(USART1, c);

		} else {
			/* queue empty */
			/* Disable the USARTy Transmit interrupt */
			USART_CR1(USART1) &= ~USART_CR1_TXEIE;
		}
	}
}


void usart2_isr(void)
{
	uint8_t c;

	if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) &&
			((USART_ISR(USART2) & USART_ISR_RXNE) != 0)) {
		gpio_toggle(GPIOF, GPIO0);
		c= usart_recv(USART2);
		/* Read one byte from the receive data register */
		/*FIXME optimize this*/
		if(IS_NOT_FULL(RX_BUF_SIZE, rx_buf_head[1], rx_buf_tail[1])) {
			rx_buf[1][rx_buf_head[1]]=c;
			rx_buf_head[1] = GET_NEXT(RX_BUF_SIZE, rx_buf_head[1]);
		}

	}

	if (((USART_CR1(USART2) & USART_CR1_TXEIE) != 0) &&
			((USART_ISR(USART2) & USART_ISR_TXE) != 0)) {
		/*check if there is something in the queue to transmit */

		if(IS_NOT_EMPTY(tx_buf_head[1],tx_buf_tail[1]))
		{
			c = tx_buf[1][tx_buf_tail[1]];
			tx_buf_tail[1] = GET_NEXT(TX_BUF_SIZE,tx_buf_tail[1]);

			/* Write one byte to the transmit data register */
			usart_send(USART2, c);

		} else {
			/* queue empty */
			/* Disable the USARTy Transmit interrupt */
			USART_CR1(USART2) &= ~USART_CR1_TXEIE;
		}
	}
}

void usart3_4_isr(void)
{
	uint8_t c;

	if (((USART_CR1(USART3) & USART_CR1_RXNEIE) != 0) &&
			((USART_ISR(USART3) & USART_ISR_RXNE) != 0)) {
		gpio_toggle(GPIOF, GPIO0);
		c= usart_recv(USART3);
		/* Read one byte from the receive data register */
		/*FIXME optimize this*/
		if(IS_NOT_FULL(RX_BUF_SIZE, rx_buf_head[2], rx_buf_tail[2])) {
			rx_buf[2][rx_buf_head[2]]=c;
			rx_buf_head[2] = GET_NEXT(RX_BUF_SIZE, rx_buf_head[2]);
		}

	}

	if (((USART_CR1(USART3) & USART_CR1_TXEIE) != 0) &&
			((USART_ISR(USART3) & USART_ISR_TXE) != 0)) {
		/*check if there is something in the queue to transmit */

		if(IS_NOT_EMPTY(tx_buf_head[2],tx_buf_tail[2]))
		{
			c = tx_buf[2][tx_buf_tail[2]];
			tx_buf_tail[2] = GET_NEXT(TX_BUF_SIZE,tx_buf_tail[2]);

			/* Write one byte to the transmit data register */
			usart_send(USART3, c);

		} else {
			/* queue empty */
			/* Disable the USARTy Transmit interrupt */
			USART_CR1(USART3) &= ~USART_CR1_TXEIE;
		}
	}

	if (((USART_CR1(USART4) & USART_CR1_RXNEIE) != 0) &&
			((USART_ISR(USART4) & USART_ISR_RXNE) != 0)) {
		gpio_toggle(GPIOF, GPIO0);
		c= usart_recv(USART4);
		/* Read one byte from the receive data register */
		/*FIXME optimize this*/
		if(IS_NOT_FULL(RX_BUF_SIZE, rx_buf_head[3], rx_buf_tail[3])) {
			rx_buf[3][rx_buf_head[3]]=c;
			rx_buf_head[3] = GET_NEXT(RX_BUF_SIZE, rx_buf_head[3]);
		}

	}

	if (((USART_CR1(USART4) & USART_CR1_TXEIE) != 0) &&
			((USART_ISR(USART4) & USART_ISR_TXE) != 0)) {
		/*check if there is something in the queue to transmit */

		if(IS_NOT_EMPTY(tx_buf_head[3],tx_buf_tail[3]))
		{
			c = tx_buf[3][tx_buf_tail[3]];
			tx_buf_tail[3] = GET_NEXT(TX_BUF_SIZE,tx_buf_tail[3]);

			/* Write one byte to the transmit data register */
			usart_send(USART4, c);

		} else {
			/* queue empty */
			/* Disable the USARTy Transmit interrupt */
			USART_CR1(USART4) &= ~USART_CR1_TXEIE;
		}
	}

}

/* ---------------- Global Functions --------------- */

void send_char(uint8_t u, uint8_t c)
{
	/*FIXME optimize this*/
	while(!IS_NOT_FULL(TX_BUF_SIZE, tx_buf_head[u], tx_buf_tail[u]));
	if(IS_NOT_FULL(TX_BUF_SIZE, tx_buf_head[u], tx_buf_tail[u])) {
		tx_buf[u][tx_buf_head[u]]=c;
		tx_buf_head[u] = GET_NEXT(TX_BUF_SIZE, tx_buf_head[u]);
	}
	switch (u){
		case NORTH:
	    /* Enable the USARTy Transmit interrupt */
		USART_CR1(USART_NORTH) |= USART_CR1_TXEIE;
		break;
		case EAST:
	    /* Enable the USARTy Transmit interrupt */
		USART_CR1(USART_EAST) |= USART_CR1_TXEIE;
		break;
		case SOUTH:
	    /* Enable the USARTy Transmit interrupt */
		USART_CR1(USART_SOUTH) |= USART_CR1_TXEIE;
		break;
		case WEST:
	    /* Enable the USARTy Transmit interrupt */
		USART_CR1(USART_WEST) |= USART_CR1_TXEIE;
		break;
	}
}


uint32_t get_char(uint8_t u)
{
	uint32_t c;
	if(IS_NOT_EMPTY(rx_buf_head[u],rx_buf_tail[u]))
	{
		c = rx_buf[u][rx_buf_tail[u]];
		rx_buf_tail[u] = GET_NEXT(RX_BUF_SIZE,rx_buf_tail[u]);
		return c;
	}
	return (uint32_t)-1;
}

#if 0
static void gpio_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOF);
	gpio_mode_setup(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0);
}
#endif

void usart_init(void)
{

//	rcc_clock_setup_in_hsi_out_48mhz();

//	gpio_setup();
	usart_setup(
			USART_NORTH,
			USART_NORTH_RCC,
			USART_NORTH_IRQ,
			USART_NORTH_RX_RCC,
			USART_NORTH_RX_PORT,
			USART_NORTH_RX_PIN,
			USART_NORTH_RX_AF_NUM,
			USART_NORTH_TX_RCC,
			USART_NORTH_TX_PORT,
			USART_NORTH_TX_PIN,
			USART_NORTH_TX_AF_NUM);

	usart_setup(
			USART_EAST,
			USART_EAST_RCC,
			USART_EAST_IRQ,
			USART_EAST_RX_RCC,
			USART_EAST_RX_PORT,
			USART_EAST_RX_PIN,
			USART_EAST_RX_AF_NUM,
			USART_EAST_TX_RCC,
			USART_EAST_TX_PORT,
			USART_EAST_TX_PIN,
			USART_EAST_TX_AF_NUM);

	usart_setup(
			USART_SOUTH,
			USART_SOUTH_RCC,
			USART_SOUTH_IRQ,
			USART_SOUTH_RX_RCC,
			USART_SOUTH_RX_PORT,
			USART_SOUTH_RX_PIN,
			USART_SOUTH_RX_AF_NUM,
			USART_SOUTH_TX_RCC,
			USART_SOUTH_TX_PORT,
			USART_SOUTH_TX_PIN,
			USART_SOUTH_TX_AF_NUM);

	usart_setup(
			USART_WEST,
			USART_WEST_RCC,
			USART_WEST_IRQ,
			USART_WEST_RX_RCC,
			USART_WEST_RX_PORT,
			USART_WEST_RX_PIN,
			USART_WEST_RX_AF_NUM,
			USART_WEST_TX_RCC,
			USART_WEST_TX_PORT,
			USART_WEST_TX_PIN,
			USART_WEST_TX_AF_NUM);
}
#if 0
void main(void){
	usart_init();
	usart_send_blocking(USART_NORTH,'x');
	send_char(NORTH, 'N');
	send_char(EAST, 'E');
	send_char(SOUTH, 'S');
	send_char(WEST, 'W');

	/* Wait forever and do nothing. */
	while (1){
		uint32_t c;
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		__asm__("nop");
		c = get_char(NORTH);
		if( c != (uint32_t)-1) {
			send_char(NORTH,(uint8_t)c);
		}
			c = get_char(EAST);
		if( c != (uint32_t)-1) {
			send_char(EAST,(uint8_t)c);
		}
				c = get_char(SOUTH);
		if( c != (uint32_t)-1) {
			send_char(SOUTH,(uint8_t)c);
		}
				c = get_char(WEST);
		if( c != (uint32_t)-1) {
			send_char(WEST,(uint8_t)c);
		}
	
	
	}

	return 0;
}
#endif
