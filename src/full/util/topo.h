#ifndef TOPO_H
#define TOPO_H

#include <stdint.h>
#include <stdbool.h>
#include <hw/usart_buffered.h>
#include <util/vector.h>

typedef struct cell_info_str {
	vector2 pos;
	uint8_t id;
	enum direction dir;
} cell_info;

#define MASTER_CELL_ID 0

extern uint8_t topo_my_id;
extern bool topo_is_master;
extern cell_info topo_cells[];
extern size_t topo_cell_count;
extern enum direction topo_master_direction;
extern enum direction topo_master_orientation;
extern vector2 topo_master_position;

void topo_run(void);
void route_message(bool send_local, msg_header *hdr, buf_ptr *buf);
void send_routed_msg(bool send_local, uint8_t id, uint8_t flags, uint8_t len, void *data);

/**
 * Positions are sent accross uart as vector relative to this cell, with the outgoing direction as
 * the "up" direction.
 *
 * This function rotates the vector appropriately.
 */
vector2 pos_tx(vector2 v, enum direction d);

/**
 * See \ref pos_tx for description.
 *
 * This functions rotates and translates the vector appropriately.
 */
vector2 pos_rx(vector2 v, enum direction d);

enum direction dir_tx(enum direction value, enum direction d);
enum direction dir_rx(enum direction value, enum direction d);

#endif // TOPO_H
