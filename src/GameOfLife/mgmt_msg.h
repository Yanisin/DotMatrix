#ifndef MGMT_MSG_H
#define MGMT_MSG_H

#include <stdint.h>
#include "cell_id.h"
#include "usart_buffered.h"

#define MGMT_ROUTE_ANNOUNCE MSG_MGMT_ID(0)
struct mgmt_route_announce {
	uint8_t distance;
	cell_id_t id;
};

#define MGMT_CHILD_ANNOUNCE MSG_MGMT_ID(1)
struct mgmt_child_announce {
	/* This is me and announcements are done */
	uint8_t final;
	/* Position of the cell that has just joined */
	int8_t x;
	int8_t y;
};

#define MGMT_NOT_A_CHILD MSG_MGMT_ID(3)
struct mgmt_not_a_child {
};

#define MGMT_ALLOC_IDS MSG_MGMT_ID(4)
struct mgmt_alloc_ids {
	uint8_t first_id;
	uint8_t id_count;
};

/*
 * Request all cells to change the current applet
 */
#define MGMT_CHANGE_APPLET MSG_MGMT_ID(5)
struct mgmt_change_applet {
	uint8_t applet;
};

/*
 * Request the management layer to switch to the choice applet
 */
#define MGM_INTERRTUP MSG_MGMT_ID(6)
struct mgmt_interrupt {};

#endif // MGMT_MSG_H
