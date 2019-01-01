#ifndef USART_BUFFERED_H
#define USART_BUFFERED_H

#include <libopencm3/stm32/usart.h>

extern void send_char(uint8_t u, uint8_t c);
extern uint32_t get_char(uint8_t u);
extern void usart_init(void);
extern void usart_buf_clear(int u);


#endif
