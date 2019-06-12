#pragma once

#include <stdint.h>
#include <hw/cell_id.h>
#include <util/message_queue.h>

#define TOPO_ROUTE_ANNOUNCE MSG_USER_ID(0)
struct mgmt_route_announce {
	uint8_t distance;
	cell_id_t id;
};

#define TOPO_CHILD_ANNOUNCE MSG_USER_ID(1)
struct mgmt_child_announce {
	/* This is me and announcements are done */
	uint8_t final;
	/* Position of the cell that has just joined */
	int8_t x;
	int8_t y;
};

#define TOPO_NOT_A_CHILD MSG_USER_ID(3)
struct mgmt_not_a_child {
};

#define TOPO_ALLOC_IDS MSG_USER_ID(4)
struct mgmt_alloc_ids {
	/* Range of IDs allocated to this cell and its children */
	uint8_t first_id;
	uint8_t id_count;
	/* Position of the master */
	int8_t master_x;
	int8_t master_y;
	/* Rotation of the master */
	uint8_t master_dir;
};
