#ifndef USART_BUFFERED_H
#define USART_BUFFERED_H

#include <libopencm3/stm32/usart.h>


#define USART_TX1_BUF_SZ 16
#define USART_TX2_BUF_SZ 16
#define USART_TX3_BUF_SZ 16
#define USART_TX4_BUF_SZ 16

#define USART_RX1_BUF_SZ 16
#define USART_RX2_BUF_SZ 16
#define USART_RX3_BUF_SZ 16
#define USART_RX4_BUF_SZ 16

void send_char(uint8_t u, uint8_t c);
uint32_t get_char(uint8_t u);
void usart_init(void);

#endif
