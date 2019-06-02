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

#define MSG_ID_FLAG_MGMT 0x80
#define MSG_ID_MASK 0x3F

#define MSG_MGMT_ID(x) (((x) & MSG_ID_MASK) | MSG_ID_FLAG_MGMT)
#define MSG_USER_ID(x) ((x) & MSG_ID_MASK)

typedef struct usart_header_str {
	uint8_t usart;
	uint8_t id;
	uint8_t length;
	uint8_t remaining;
	uint8_t available;
} usart_header;

typedef bool (*usart_msg_dispatcher)(const usart_header *msg);

void usart_init(void);
void usart_buf_clear(int u);

/**
 * Check the current status of the USART buffer.
 *
 * Usually, the applet does not need to use this funciton, since it will be handled by
 * \ref usart_recv_dispatch
 *
 * @returns NULL if no message is currently in the buffer
 */
const usart_header* usart_peek(uint8_t usart);

/**
 * Receive data from all serial port and if there is any message, dispatch it
 * either to the management code or to the currently running applet, as appropriate.
 *
 * @see applet#check_usart
 */
void usart_recv_dispatch(usart_msg_dispatcher dispatch);


/**
 * Check if the complete message is buffered already.
 */
static inline bool usart_is_complete(usart_header *hdr)
{
	return hdr->remaining == hdr->available;
}

/**
 * Consume the content of the current message (see usart_peek).
 *
 * The \a buf argument must provide enough memory to hold \ref usart_header#length bytes.
 * If there is not enough memory, the message is skipped instead.
 *
 * @return true if the message was received
 * @return false if the message is longer than \a size
 * @return false if the message is not completely buffered
 */
bool usart_recv_msg(uint8_t usart, void *buf, size_t size);

void usart_skip(uint8_t usart);

/**
 * Consume part of the content of the current message.
 *
 * @returns number of bytes read
 */
uint8_t usart_recv_partial(uint8_t usart, void *buf, size_t size);

void usart_send_msg(uint8_t usart, uint8_t id, uint8_t len, const void *data);
void usart_send_char(uint8_t u, uint8_t c);

#ifdef SIM
void usart_sim_recv(uint8_t usart, const uint8_t *data, size_t len);
#endif

#endif
