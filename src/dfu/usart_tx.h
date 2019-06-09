#ifndef USART_BUFFERED_H
#define USART_BUFFERED_H

#include <stdint.h>

typedef struct {
	uint8_t len;
	uint8_t id;
} msg_header;

void usart_init(void);
void usart_reset_neigbors(void);

#endif // USART_BUFFERED_H
