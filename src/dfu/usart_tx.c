#include <stdint.h>
#include <stdbool.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include "hw_defs.h"
#include "usart_tx.h"
#include "../full/util.h"

#define USART_COUNT 4
#define MSG_ID_FLAG_MGMT 0x20
#define MSG_ID_MASK 0x1F

/* ---------------- Local Variables --------------- */
static struct usart {
	uint32_t reg_base;
} usarts[USART_COUNT];

static uint8_t reset_msg[3] = {
	0, // length
	0x20, // MSG BL START
	0 // crc will be filled in
};

/* ---------------- Local Functions --------------- */
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
	(void) irq;
	usarts[usart_num].reg_base = usart;
	rcc_periph_clock_enable(usart_rcc);
	rcc_periph_clock_enable(rx_rcc);
	rcc_periph_clock_enable(tx_rcc);

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

	/* Finally enable the USART. */
	usart_enable(usart);
}

/* ---------------- Global Functions --------------- */

static bool usart_all_ready(void)
{
	for(int i = 0; i < USART_COUNT; i++) {
		struct usart *usart = &usarts[i];
		if ((USART_ISR(usart->reg_base) & USART_ISR_TXE) == 0)
			return false;
	}
	return true;
}

static void usart_send_all_char(uint8_t c)
{

	for(int i = 0; i < USART_COUNT; i++) {
		struct usart *usart = &usarts[i];
		usart_send(usart->reg_base, c);
	}
}

void usart_reset_neigbors(void)
{
	uint32_t crc = 0;
	crc8(&crc, reset_msg[0]);
	crc8(&crc, reset_msg[1]);
	reset_msg[2] = crc8_get(&crc);
	for(int i = 0; i < 3; i++) {
		while (!usart_all_ready())
			continue;
		usart_send_all_char(reset_msg[i]);
	}
}

void usart_init(void)
{
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
