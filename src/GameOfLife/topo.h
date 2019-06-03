#ifndef TOPO_H
#define TOPO_H

#include <stdint.h>
#include <stdbool.h>
#include "usart_buffered.h"
#include "vector.h"

typedef struct cell_info_str {
	vector2 pos;
	uint8_t id;
	enum direction dir;
} cell_info;

extern uint8_t topo_my_id;
extern bool topo_is_master;
extern cell_info topo_cells[];
extern size_t topo_cell_count;
extern enum direction topo_master_direction;

void topo_run(void);
bool handle_topo_message(const usart_header *msg);

/**
 * @returns true if this cell is the intended recipient
 */
bool topo_send_up(uint8_t msgid, uint8_t len, const void *data);
void topo_send_down(uint8_t msgid, uint8_t len, const void *data);

/**
 * @returns true if this cell is the intended recipient
 */
bool topo_send_to(uint8_t recipient, uint8_t msgid, uint8_t len, const void *data);

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

#endif // TOPO_H
