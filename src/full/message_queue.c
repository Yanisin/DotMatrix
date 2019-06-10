#include "message_queue.h"
#include <ch.h>
#include <assert.h>
#include "console.h"
#include "pollint.h"
#include "applet.h"

#define FIELD_LENGTH 0
#define FIELD_FLAGS 1
#define GUARD_MAGIC 0x3CFE

msg_rx_queue *default_queue;
msg_rx_queue *alt_queue;
msg_rx_queue *mgmt_queue;
msg_rx_queue *usart_route_queue;

void msg_rx_queue_init(msg_rx_queue *queue, uint8_t size_bits, void *buffer)
{
	assert(size_bits < (sizeof(qsize_t) * 8));
	chEvtObjectInit(&queue->event);
	chThdQueueObjectInit(&queue->readers);
	queue->size_bits = size_bits;
	queue->buffer = buffer;
	queue->read_ptr = 0;
	queue->comit_ptr = 0;
	queue->reserve_ptr = 0;
}

msg_rx_queue* msg_rx_queue_alloc(memory_heap_t *heap, uint8_t size_bits)
{
	msg_rx_queue *q = chHeapAlloc(heap, sizeof(msg_rx_queue));
	if (q == NULL)
		return q;
	void *buf = chHeapAlloc(heap, 1 << size_bits);
	if (buf == NULL) {
		chHeapFree(q);
		return buf;
	}

	msg_rx_queue_init(q, size_bits, buf);
	return q;

}

static uint8_t *queue_index(msg_rx_queue *queue, qsize_t at)
{
	return &queue->buffer[mod_bits16(queue->size_bits, at)];
}

static void buf_ptr_init(msg_rx_queue *queue, buf_ptr *buf_ptr_out, qsize_t ptr)
{
	buf_ptr_out->buffer = queue->buffer;
	buf_ptr_out->size_bits = queue->size_bits;
	buf_ptr_out->ptr = ptr;
}

void buf_ptr_read(const buf_ptr *b, qsize_t offset, qsize_t length, void *dst)
{
	for(qsize_t i = 0; i < length; i++) {
		((uint8_t*)dst)[i] = *buf_ptr_index(b, i + offset);
	}
}

void buf_ptr_write(const buf_ptr *b, qsize_t offset, qsize_t length, const void *src)
{
	for(qsize_t i = 0; i < length; i++) {
		*buf_ptr_index(b, i + offset) = ((uint8_t*)src)[i];
	}
}

void buf_ptr_copy(
	const buf_ptr *dst, qsize_t dst_offset,
	const buf_ptr *src, qsize_t src_offset,
	qsize_t length)
{
	for(qsize_t i = 0; i < length; i++) {
		*buf_ptr_index(dst, i + dst_offset) = *buf_ptr_index(src, i + src_offset);
	}
}

#ifdef DEBUG
static inline void check_message_guard(msg_rx_queue *queue, qsize_t ptr)
{
	buf_ptr b;
	msg_header h;
	buf_ptr_init(queue, &b, ptr);
	buf_ptr_read(&b, 0, sizeof(h), &h);
	if (h.guard != GUARD_MAGIC)
		chSysHalt("Queue state corrupted");
}
#endif

static inline void check_queue_state(msg_rx_queue *queue)
{
	(void) queue;
#ifdef DEBUG
	if (queue->read_ptr != queue->comit_ptr)
		check_message_guard(queue, queue->read_ptr);
	if (queue->comit_ptr != queue->reserve_ptr)
		check_message_guard(queue, queue->comit_ptr);
#endif
}

static bool msg_rx_queue_get_raw(
	msg_rx_queue *queue, msg_header *header_out, buf_ptr *buf_ptr_out, sysinterval_t timeout)
{
	//console_printf("r: %d c: %d r: %d\n", queue->read_ptr, queue->comit_ptr, queue->reserve_ptr);
	chSysLock();
	if (msg_rx_queue_available_read(queue) == 0) {
		msg_t msg = chThdEnqueueTimeoutS(&queue->readers, timeout);
		if (msg == MSG_TIMEOUT) {
			chSysUnlock();
			return false;
		}
	}
	chSysUnlock();


	assert(msg_rx_queue_available_read(queue) >= sizeof(msg_header));
	check_queue_state(queue);
	buf_ptr_init(queue, buf_ptr_out, queue->read_ptr);
	buf_ptr_read(buf_ptr_out, 0, sizeof(msg_header), header_out);
	buf_ptr_out->ptr += sizeof(msg_header);
	header_out->flags &= ~(MSG_READY);

	return true;
}

bool msg_rx_queue_get(
	msg_rx_queue *queue, msg_header *header_out, buf_ptr *buf_ptr_out, sysinterval_t timeout)
{
	systime_t start = chVTGetSystemTimeX();
	sysinterval_t ctimeout = timeout;
	do {
		poll_int();
		if (!msg_rx_queue_get_raw(queue, header_out, buf_ptr_out, ctimeout))
			return false;

		if (timeout == TIME_INFINITE) {
			ctimeout = TIME_INFINITE;
		} else {
			sysinterval_t elapsed = chVTTimeElapsedSinceX(start);
			if (elapsed >= timeout)
				ctimeout = TIME_IMMEDIATE;
			else
				ctimeout = timeout - elapsed;
		}
		if (header_out->flags & MSG_INVALID)
			msg_rx_queue_ack(queue);
	} while(header_out->flags & MSG_INVALID);
	return true;
}

void msg_rx_queue_ack(msg_rx_queue *queue)
{
	qsize_t len = *queue_index(queue,  queue->read_ptr + FIELD_LENGTH);
	queue->read_ptr += len + sizeof(msg_header);
	check_queue_state(queue);
}

bool msg_rx_queue_reserveI(msg_rx_queue *queue, const msg_header *header, buf_ptr *buf_ptr_out)
{
	qsize_t total = sizeof(*header) + header->length;
	check_queue_state(queue);
	if (msg_rx_queue_available_write(queue) < total)
		return false;
	buf_ptr_init(queue, buf_ptr_out, queue->reserve_ptr);
#ifdef DEBUG
	msg_header h2 = *header;
	h2.guard = GUARD_MAGIC;
	buf_ptr_write(buf_ptr_out, 0, sizeof(h2), &h2);
#else
	buf_ptr_write(buf_ptr_out, 0, sizeof(*header), header);
#endif
	buf_ptr_out->ptr += sizeof(*header);
	queue->reserve_ptr += total;
	return true;
}

static inline bool ready_to_commit(msg_rx_queue *queue)
{
	return (*queue_index(queue, queue->comit_ptr + FIELD_FLAGS)) & MSG_READY;
}

void msg_rx_queue_commitI(msg_rx_queue *queue, const buf_ptr *b)
{
	qsize_t header = b->ptr - sizeof(msg_header);
	bool wakeup = false;

	*queue_index(queue, header + FIELD_FLAGS) |= MSG_READY;
	if (header != queue->comit_ptr) {
		/* there are still uncommited messages before us */
		return;
	}
	// console_printf("r: %d c: %d r: %d\n", queue->read_ptr, queue->comit_ptr, queue->reserve_ptr);
	while ((queue->comit_ptr != queue->reserve_ptr) && ready_to_commit(queue)) {
		check_queue_state(queue);
		uint8_t len = queue->buffer[mod_bits16(queue->size_bits, queue->comit_ptr + FIELD_LENGTH)];
		queue->comit_ptr += sizeof(msg_header) + len;
		wakeup = true;
	}
	if (wakeup) {
		chEvtBroadcastI(&queue->event);
		chThdDequeueNextI(&queue->readers, MSG_OK);
	}
}

void msg_rx_queue_rejectI(msg_rx_queue *queue, const buf_ptr *b)
{
	chSysHalt("reject");
	qsize_t header = b->ptr - sizeof(msg_header);
	queue->buffer[mod_bits16(queue->size_bits, header + FIELD_FLAGS)] |= MSG_INVALID;
	msg_rx_queue_commitI(queue, b);
}

msg_rx_queue* msg_dispatcher(const msg_header *hdr)
{
	if (hdr->id & MSG_ID_FLAG_MGMT) {
		return mgmt_queue;
	} else if ((hdr->flags & MSG_ROUTE_MASK) != MSG_ROUTE_NEIGHBOR) {
		if (hdr->source == MSG_SOURCE_I2C) {
			console_printf("routed message over i2c");
			return NULL;
		} else {
			return usart_route_queue;
		}
	} else {
		const struct applet * a = applet_current();
		if (a && a->dispatch_msg)
			return a->dispatch_msg(hdr);
		else
			return default_queue;
	}
}

#ifdef SIM
/* We need to work-around the lack of rate limiting */
#define BUF_MULTIPLIER 2
#else
#define BUF_MULTIPLIER 0
#endif
static void init_default_queues(void) {
	default_queue = msg_rx_queue_alloc(NULL, 7 + BUF_MULTIPLIER);
	alt_queue = msg_rx_queue_alloc(NULL, 7 + BUF_MULTIPLIER);
	usart_route_queue = msg_rx_queue_alloc(NULL, 6 + BUF_MULTIPLIER);
	mgmt_queue = msg_rx_queue_alloc(NULL, 5 + BUF_MULTIPLIER);
}
init_add(init_default_queues);
