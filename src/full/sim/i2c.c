#include <string.h>
#include <stdlib.h>
#include <ch.h>
#include <hw/i2c.h>
#include <util/topo.h>
#include "sim.h"
#include "proto_defs.h"

MUTEX_DECL(i2c_mutex);
static uint8_t sbuf[300];
#define SIM_HDR_LEN 2

void i2c_init(void)
{
}

bool i2c_send(uint8_t cell, uint8_t msgid, uint8_t len, const void *data)
{
	chMtxLock(&i2c_mutex);
	sbuf[0] = cell + 1;
	sbuf[1] = msgid;
	memcpy(sbuf + SIM_HDR_LEN, data, len);
	sim_send(MSGID_I2C_TX, len + SIM_HDR_LEN, sbuf);
	chMtxUnlock(&i2c_mutex);
	return true;
}

bool i2c_broadcast(uint8_t msgid, uint8_t len, const void *data)
{
	chMtxLock(&i2c_mutex);
	sbuf[0] = 0;
	sbuf[1] = msgid;
	memcpy(sbuf + SIM_HDR_LEN, data, len);
	sim_send(MSGID_I2C_TX, len + SIM_HDR_LEN, sbuf);
	chMtxUnlock(&i2c_mutex);
	return true;
}

void i2c_sim_recv(uint16_t len, const uint8_t *data)
{
	msg_header hdr;
	buf_ptr buf;
	msg_rx_queue *q;
	bool match = data[0] == 0 || (data[0] - 1) == topo_my_id;
	if (!match)
		return;
	hdr.length = len - SIM_HDR_LEN;
	hdr.id = data[1];
	hdr.flags = 0;
	hdr.source = MSG_SOURCE_I2C;
	chSysLockFromISR();
	q = msg_dispatcher(&hdr);
	if (q) {
		if (msg_rx_queue_reserveI(q, &hdr, &buf)) {
			buf_ptr_write(&buf, 0, hdr.length, data + SIM_HDR_LEN);
			msg_rx_queue_commitI(q, &buf);
		} else {
			chSysHalt("i2c overflow");
		}
	}
	chSysUnlockFromISR();
}
