#ifndef TOPO_H
#define TOPO_H

#include <stdint.h>
#include <stdbool.h>
#include <hw/usart_buffered.h>
#include <util/vector.h>

/**
 * @file
 * Provides results of the topology detection phase and some useful topology functions.
 *
 * Topology detection builds a spanning tree of the cells and elects a master.
 */

/**
 * Information about a cell (one square).
 */
typedef struct cell_info_str {
	/** Position of the cell relative to our cell */
	vector2 pos;
	/** Assigned id of the cell */
	uint8_t id;
	/** Rotation of the cell (the direction it is facing) relative to our cell */
	enum direction dir;
} cell_info;

/**
 * Type of the neighbor connected on the given cell edge.
 */
enum edge_type {
	/** There is no neighbor */
	EDGE_DEAD,
	/**
	 * Internal state value
	 *
	 * ANNOUNCE_CELLS has not yet run, we ca not tell. We just know it is live and not root.
	 */
	EDGE_LIVE_UNKNOWN,
	/** The cell is our connection to master. */
	EDGE_MASTER,
	/** One of our children (we are their connection to master) */
	EDGE_CHILD,
	/** A regular neighbor */
	EDGE_NORMAL
};

typedef struct edge_info_str {
	enum edge_type type;
	bool announcements_end;
	uint8_t children_count;
	uint8_t first_cell;
} edge_info;

/** Assigned id of the master cell */
#define MASTER_CELL_ID 0

/** Information about our immediate neighbors */
extern edge_info topo_edges[DIR_COUNT];
/** ID of our cell */
extern uint8_t topo_my_id;
/** Are we the master cell */
extern bool topo_is_master;
/**
 * Information about other cells.
 *
 * Please note that only the master will know about all cells, since this list contains only
 * the children of this cell in the spanning tree.
 */
extern cell_info topo_cells[];
/** Number of cells in topo_cells array */
extern size_t topo_cell_count;
/** The edge we use to connect to master */
extern enum direction topo_master_direction;
/** Rotation of the master cell relative to us */
extern enum direction topo_master_orientation;
/** Position of the master cell relative to us */
extern vector2 topo_master_position;

/**
 * Run the topology phase (automatically called during boot)
 */
void topo_run(void);
/**
 * Take a message from rx_queue and re-route it to other system or a local destination.
 */
void route_message(bool send_local, msg_header *hdr, buf_ptr *buf);

/**
 * Take a message route it to other system or a local destination.
 */
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

/**
 * Transform direction from local (relative to the current cell) to global (relative to master).
 */
enum direction global_direction(enum direction value);

#endif // TOPO_H
