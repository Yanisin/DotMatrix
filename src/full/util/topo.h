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

enum edge_type {
	EDGE_DEAD,
	/* ANNOUNCE_CELLS has not yet run, we ca not tell. We just know it is live and not root. */
	EDGE_LIVE_UNKNOWN,
	/* Our route to the master */
	EDGE_MASTER,
	/* One of our children */
	EDGE_CHILD,
	/* A regular neighbor */
	EDGE_NORMAL
};

typedef struct edge_info_str {
	enum edge_type type;
	bool announcements_end;
	uint8_t children_count;
	uint8_t first_cell;
} edge_info;


#define MASTER_CELL_ID 0

extern edge_info topo_edges[DIR_COUNT];
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
