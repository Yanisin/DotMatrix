#pragma once
#include <ch.h>
#include "vector.h"
#include <chmemheaps.h>

#define GUARD_MAGIC 0x3CFE
#define GUARD_DESTROYED 0x3CFE

/** Used for offsets and sizes inside the message queue */
typedef uint16_t qsize_t;

/** Source of the message in message queue. */
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

/** Header of a message in a message queue. */
typedef struct msg_header_str {
	/* Do not reorganize field order wihtout checking message queue code */
	/** Length of the data part of the message */
	uint8_t length;
	/** Additional flags, see definitions below */
	uint8_t flags;
#ifdef DEBUG
	/** For checking memory corruptions inside a queue */
	uint16_t guard;
#endif
	/** From which device came this message?
	 * One of the message_source enumeration values */
	uint8_t source;
	/** Message type ID assigned to the user. */
	uint8_t id;
} msg_header;

/** The message is to be proccessed by the management code, not by the applet */
#define MSG_ID_FLAG_MGMT 0x20
/** Valid bits for a message ID */
#define MSG_ID_MASK 0x1F
/** Valid bits for a message ID including the management/user flag. */
#define MSG_FULL_ID_MASK (MSG_ID_MASK | MSG_ID_FLAG_MGMT)

/** Use this macro to define a message type intended for management */
#define MSG_MGMT_ID(x) (((x) & MSG_ID_MASK) | MSG_ID_FLAG_MGMT)
/** Use this macro to define a message type for applets. */
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
 * Represents a section of a circular buffer (such as rxqueue).
 */
typedef struct buf_ptr_str {
#ifdef DEBUG
	uint16_t guard;
#endif
	uint8_t *buffer;
	uint8_t size_bits;
	qsize_t ptr;
} buf_ptr;

/** Default queue for all non-management messages */
extern msg_rx_queue * const default_queue;
/**
 * If an applet wishes to receive messages on this queue, it must use it manually,
 * using applet#dispatch_msg.
 */
extern msg_rx_queue * const alt_queue;
/**
 * Internal queue used by the system for management messages.
 */
extern msg_rx_queue * const mgmt_queue;
/**
 * Internal queue used by the system for messages that will be automatically re-sent
 * according to their routing information.
 */
extern msg_rx_queue * const usart_route_queue;

/** Determines into which queue a message should be placed. Typically used by HW drivers
 * placing messages into queues.
 */
msg_rx_queue* msg_dispatcher(const msg_header *hdr);

static inline qsize_t mod_bits16(uint8_t bits, qsize_t x)
{
	qsize_t mask = (1 << bits) - 1;
	return x & mask;
}

/**
 * Initialize a new message queue with already allocated storage
 * @param size_bits Logarithmic size of the queue. The actual size is 1 << size_bites.
 * @param buffer Storage for the message, must be at least 1 << size_bytes large
 */
void msg_rx_queue_init(msg_rx_queue *queue, uint8_t size_bits, void *buffer);
/**
 * Initialize new message queue with message storage on the heap.
 */
msg_rx_queue* msg_rx_queue_alloc(memory_heap_t *heap, uint8_t size_bits);
/**
 * Free previously allocated queue.
 * Only use with queues allocated by msg_rx_queue_init.
 */
void msg_rx_queue_free(msg_rx_queue *queue);
/**
 * Wait for first message in the queue and retrieve it. Does not consume the message.
 *
 * @remark The function will clobber \p header_out and \p buf_ptr_out even if not succesfull.
 * @param buf_ptr_out Pointer to message header. Valid until msg_rx_queue_ack is called.
 * @returns true if there is a valid message
 * @return false if no message appeared during the time-out. \p header_out and \p buf_ptr_out
 *	         are not valid in that case.
 */
bool msg_rx_queue_get(
	msg_rx_queue *queue, msg_header *header_out, buf_ptr *buf_ptr_out, sysinterval_t timeout);
/**
 * Consume the first message in the queue. The user must first call msg_rx_queue_get sucesfully to
 * ensure there is a message.
 */
void msg_rx_queue_ack(msg_rx_queue *queue);

/**
 * Reserve space for writing the message into the queue and write the header.
 *
 * The message header indicates how much space needs to be allocated. The caller
 * must use the buffer API to write the actual data between this call and
 * msg_rx_queue_commitI or msg_rx_queue_rejectI.
 *
 * @param header Header of the message that the caller wants to write.
 * @param buf_ptr_out Space allocate for the the message
 */
bool msg_rx_queue_reserveI(msg_rx_queue *queue, const msg_header *header, buf_ptr *buf_ptr_out);

/**
 * Commit a previously reserved message.
 *
 * @param buf_ptr message data reserved by msg_rx_queue_reserveI
 */
void msg_rx_queue_commitI(msg_rx_queue *queue, buf_ptr *buf_ptr);
/**
 * Cancel previous reservation of a message.
 *
 * @param buf_ptr message data reserved by msg_rx_queue_reserveI
 */
void msg_rx_queue_rejectI(msg_rx_queue *queue, buf_ptr *buf_ptr);

/**
 * Returns number of bytes that can be read from the queue.
 *
 * If it is > 0, at least one message is ready.
 */
static inline qsize_t msg_rx_queue_available_read(const msg_rx_queue* queue)
{
	return mod_bits16(queue->size_bits, queue->comit_ptr - queue->read_ptr);
}

/**
 * Returns number of bytes that are free (available for writing) in the queue.
 */
static inline qsize_t msg_rx_queue_available_write(const msg_rx_queue* queue)
{
	/* One empty byte is reserved so that we avoid read_ptr == comit_ptr abiquity */
	return mod_bits16(queue->size_bits, queue->read_ptr - queue->comit_ptr) - 1;
}

/** Internal consistency check. The user does not have to use this API. */
static inline void buf_ptr_check_guard(const buf_ptr *b)
{
	(void)b;
#ifdef DEBUG
	if (b->guard != GUARD_MAGIC) {
		chSysHalt("buffer corrupted");
	}
#endif
}

/**
 * Return pointer to the \p i th byte in the circular buffer \p b.
 */
static inline uint8_t *buf_ptr_index(const buf_ptr *b, qsize_t i)
{
	buf_ptr_check_guard(b);
	return &b->buffer[mod_bits16(b->size_bits, b->ptr + i)];
}

/**
 * Read data from circular buffer section to linear memory.
 */
void buf_ptr_read(const buf_ptr *b, qsize_t offset, qsize_t length, void *dst);
/** Write data from linear memory into circular buffer section*/
void buf_ptr_write(const buf_ptr *b, qsize_t offset, qsize_t length, const void *src);
/**
 * Copy data from one circular buffer section to another.
 */
void buf_ptr_copy(
	const buf_ptr *dst, qsize_t dst_offset,
	const buf_ptr* src, qsize_t src_offset,
	qsize_t length);
