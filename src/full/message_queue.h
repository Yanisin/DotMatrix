#pragma once
#include <ch.h>
#include "vector.h"
#include <chmemheaps.h>

#define GUARD_MAGIC 0x3CFE
#define GUARD_DESTROYED 0x3CFE

typedef uint16_t qsize_t;

enum message_source {
	MSG_SOURCE_USART_UP = DIR_UP,
	MSG_SOURCE_USART_RIGHT = DIR_RIGHT,
	MSG_SOURCE_USART_DOWN = DIR_DOWN,
	MSG_SOURCE_USART_LEFT = DIR_LEFT,
	MSG_SOURCE_I2C = 5,
	MSG_SOURCE_COUNT
};

/**
 * Routing information is used for UART messages.
 */
#define MSG_ROUTE_SHIFT 0u
#define MSG_ROUTE_MASK  (0x3u << MSG_ROUTE_SHIFT)
/**
 * This message is addressed to the direct neighbor.
 *
 * If the message should be propageted, the application should handle that.
 */
#define MSG_ROUTE_NEIGHBOR 0u
/**
 * This message is addressed to all cells.
 */
#define MSG_ROUTE_BROADCAST 1u
/**
 * This message is addressed to a single cell. First byte of the message must contain the cell ID.
 */
#define MSG_ROUTE_UNICAST 2u
/**
 * This message is addressed to the master and should be sent up the spanning tree.
 */
#define MSG_ROUTE_MASTER 3u
/**
 * This message is invalid (bad CRC, incomplete reception) and should be discarded.
 */
#define MSG_INVALID (1u << 2)
/**
 * Internal flag
 */
#define MSG_READY (1u << 3)

#define MSG_DEFAULT 0

typedef struct msg_header_str {
	/* Do not reorganize field order wihtout checking message queue code */
	uint8_t length;
	uint8_t flags;
#ifdef DEBUG
	uint16_t guard;
#endif
	uint8_t source;
	uint8_t id;
} msg_header;

typedef struct msg_out_header_str {
	uint8_t length;
	uint8_t flags;
	uint8_t id;
} msg_out_header;

#define MSG_ID_FLAG_MGMT 0x20
#define MSG_ID_MASK 0x1F
#define MSG_FULL_ID_MASK (MSG_ID_MASK | MSG_ID_FLAG_MGMT)

#define MSG_MGMT_ID(x) (((x) & MSG_ID_MASK) | MSG_ID_FLAG_MGMT)
#define MSG_USER_ID(x) ((x) & MSG_ID_MASK)

/**
 * Receive queue (circular buffer) with multiple writer support.
 *
 * Each message must start with \ref msg_header header.
 *
 * Multiple writers are supported by first allocating space in the queue and committing the message
 * later. If the writer finds out that the message can not be commited, the space is not reclaimed,
 * but message is marked as failed.
 *
 * Only single reader is supported.
 */
typedef struct msg_rx_queue_str {
	/**
	 * Event is signalled when queue becomes non empty (first message is commited).
	 *
	 * No specific event flags are used.
	 */
	event_source_t event;
	threads_queue_t readers;
	uint8_t *buffer;
	uint8_t size_bits;
	/* read_ptr to reserve_ptr => messages that can be read by user */
	qsize_t read_ptr;
	/* commit_ptr to reserve_ptr => messages about to be written */
	qsize_t comit_ptr;
	/* reserver to read_ptr => empty space */
	qsize_t reserve_ptr;
} msg_rx_queue;

/**
 * Handle for reading/writing a message stored in a circular buffer (such as rx queue).
 */
typedef struct buf_ptr_str {
#ifdef DEBUG
	uint16_t guard;
#endif
	uint8_t *buffer;
	uint8_t size_bits;
	qsize_t ptr;
} buf_ptr;


extern msg_rx_queue * const default_queue;
/** If an applet wishes to receive messages on this queue, it must enable it manually */
extern msg_rx_queue * const alt_queue;
extern msg_rx_queue * const mgmt_queue;
extern msg_rx_queue * const usart_route_queue;
msg_rx_queue* msg_dispatcher(const msg_header *hdr);

static inline qsize_t mod_bits16(uint8_t bits, qsize_t x)
{
	qsize_t mask = (1 << bits) - 1;
	return x & mask;
}

void msg_rx_queue_init(msg_rx_queue *queue, uint8_t size_bits, void *buffer);
msg_rx_queue* msg_rx_queue_alloc(memory_heap_t *heap, uint8_t size_bits);
void msg_rx_queue_free(msg_rx_queue *queue);
bool msg_rx_queue_get(
	msg_rx_queue *queue, msg_header *header_out, buf_ptr *buf_ptr_out, sysinterval_t timeout);
void msg_rx_queue_ack(msg_rx_queue *queue);
bool msg_rx_queue_reserveI(msg_rx_queue *queue, const msg_header *header, buf_ptr *buf_ptr_out);
void msg_rx_queue_commitI(msg_rx_queue *queue, buf_ptr *buf_ptr);
void msg_rx_queue_rejectI(msg_rx_queue *queue, buf_ptr *buf_ptr);

static inline qsize_t msg_rx_queue_available_read(const msg_rx_queue* queue)
{
	return mod_bits16(queue->size_bits, queue->comit_ptr - queue->read_ptr);
}

static inline qsize_t msg_rx_queue_available_write(const msg_rx_queue* queue)
{
	/* One empty byte is reserved so that we avoid read_ptr == comit_ptr abiquity */
	return mod_bits16(queue->size_bits, queue->read_ptr - queue->comit_ptr) - 1;
}

static inline void buf_ptr_check_guard(const buf_ptr *b)
{
	(void)b;
#ifdef DEBUG
	if (b->guard != GUARD_MAGIC) {
		chSysHalt("buffer corrupted");
	}
#endif
}

static inline uint8_t *buf_ptr_index(const buf_ptr *b, qsize_t i)
{
	buf_ptr_check_guard(b);
	return &b->buffer[mod_bits16(b->size_bits, b->ptr + i)];
}

void buf_ptr_read(const buf_ptr *b, qsize_t offset, qsize_t length, void *dst);
void buf_ptr_write(const buf_ptr *b, qsize_t offset, qsize_t length, const void *src);
void buf_ptr_copy(
	const buf_ptr *dst, qsize_t dst_offset,
	const buf_ptr* src, qsize_t src_offset,
	qsize_t length);
