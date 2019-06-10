#ifndef USART_BUFFERED_H
#define USART_BUFFERED_H

/**
 * @file
 *
 * Module for receiving and buffering USART messages.
 *
 * The module provides framing and message IDs. These features are needed so that the USART
 * can be shared between the management code and applets themselves.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "message_queue.h"

typedef struct usart_header_str {
	uint8_t usart;
	uint8_t id;
	uint8_t length;
	uint8_t remaining;
	uint8_t available;
} usart_header;

void usart_send_msg(uint8_t u, uint8_t id, uint8_t flags, uint8_t length, const void *data);
void usart_send_msg_buf(uint8_t u, uint8_t id, uint8_t flags, uint8_t length, const buf_ptr *src);

#ifdef SIM
void usart_sim_recv(uint8_t usart, const uint8_t *data, size_t len);
#endif

#endif
