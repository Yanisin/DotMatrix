#include <string.h>
#include <ch.h>
#include "topo.h"
#include "cell_id.h"
#include "console.h"
#include "usart_buffered.h"
#include "util.h"
#include "topo_proto.h"

#define MAX_CELL_COUNT 64
#define FIND_SS_TIMEOUT TIME_MS2I(500)
#define TOPO_TOTAL_TIMEOUT TIME_MS2I(2000)
#define REANNOUNCE TIME_MS2I(100)
//#define TRACE

enum topo_state {
	/* In this phase, the spanning tree is established by. Each cell broadcasts in how many
	 * steps it can reach its master. The algorithm starts by each cell announcing that it can
	 * reach itself in zero steps. If the cell receives this announcement message with higher
	 * master id, it uses the new route. It also uses the route if it is shorter, but to the same
	 * master.
	 *
	 * The phase ends when a time-out is reached or when a message from a next phase is received.
	 * All cells should have routes to master by end of this phase, thus forming a spanning tree,
	 * with the master as a root.
	 *
	 * The cells also learn what neigbours are live and who are not in this phase.
	 */
	FIND_SS,
	/* In this step, the cells gather the physical topology from "bottom up".
	 *
	 * Each cell first sends MGMT_NOT_A_CHILD message to all live neighbors, except the one that
	 * is part of its route to master. When cell receives such a message from direction other than
	 * the route to master, they know these edges are not part of the spanning tree. Each cell
	 * thus knows if its edge is:
	 * 1) Not part of the cell graph (no message in FIND_SS stage)
	 * 2) Outgoing spanning tree edge (path to master)
	 * 3) Not part of the spanning tree (received NOT_A_CHILD message, but not #2)
	 * 4) Incomming spanning tree edge (neither #2 or #3)
	 *
	 * The cells listen for CHILD_ANNOUNCE,  and NOT_A_CHILD messages. Once all
	 * its live neigbors have sent final CHILD_ANNOUNCE or NOT_A_CHILD messages, it
	 * re-sends the CHILD_ANNOUNCE messages, add its own announcement and then send
	 * CHILD_ANNOUNCE_END.
	 *
	 * Note: Buffering of CHILD_ANNOUNCE cated ws the cell to know how may children it has in each
	 * direction and assing consecutive ids to each direction.
	 */
	ANNOUNCE_CHILDREN,
	/*
	 * Cells sub-allocate ids in top-down fashion. Master sends the ALLOC_IDS and cells
	 * sub-allocate them to their children.
	 *
	 * Note: this phase is currently not strictly necessary, but allows the cell to know
	 * its id, which may come in handy.
	 */
	ALLOCATE_IDS,
	DONE
};

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

static void announce_route(void);
static void announce_not_a_child(void);
static void topo_dispatch(const msg_header *hdr, const buf_ptr *data);
static void transition_announce_children(void);
static void send_ids(void);

/* Transition to ALLOCATE_IDS if appropriate */
static bool check_announcements_end(void);
/* Make sure the cells are consecutive w.r.t. direction in the cells array */
static void sort_children(void);
static void announce_children(void);
static void route_thread_run(void *p);

uint8_t topo_my_id;
bool topo_is_master;
cell_info topo_cells[MAX_CELL_COUNT];
size_t topo_cell_count;
enum direction topo_master_direction;

static edge_info edges[DIR_COUNT];
static cell_info sort_cells[MAX_CELL_COUNT];
static uint8_t first_cell_id;
static enum topo_state state;

static uint8_t master_usart;
static uint8_t master_distance;
static cell_id_t master_cell_id;
enum direction topo_master_orientation;
vector2 topo_master_position;

static THD_WORKING_AREA(route_thread_area, 256);

void topo_run(void)
{
	msg_header msg;
	buf_ptr data;
	systime_t topo_start_tick;
	systime_t next_announce;
	systime_t next_end;
	systime_t next_ss_end;

	topo_is_master = true;
	master_cell_id = *get_cell_id();
	master_distance = 0;
	state = FIND_SS;

	cell_info *me = &topo_cells[topo_cell_count++];
	me->dir = DIR_COUNT;
	me->pos = mk_vector2(0, 0);

	chThdSleep(TIME_MS2I(200));

	topo_start_tick = chVTGetSystemTime();
	next_announce = 0;
	next_end = topo_start_tick + TOPO_TOTAL_TIMEOUT;
	next_ss_end = topo_start_tick + FIND_SS_TIMEOUT;
#ifdef TRACE
	enum topo_state prev = DONE;
#endif

	while (state != DONE) {
		systime_t now = chVTGetSystemTime();
		systime_t next_event = MIN(next_ss_end, MIN(next_announce, next_end));
		sysinterval_t timeout;
#ifdef TRACE
		if (prev != state) {
			console_printf("state %d, now = %u\n", state, now);
			prev = state;
		}
#endif
		if (state == FIND_SS && next_announce <= now) {
			announce_route();
			next_announce = now + REANNOUNCE;
		}

		if (state == FIND_SS && next_ss_end <= now) {
			transition_announce_children();
			next_ss_end = TIME_INFINITE;
			next_announce = TIME_INFINITE;
		}

		if (next_end <= now) {
			chSysHalt("Failed complete topology detection");
		}

		timeout = (next_event < now) ? TIME_IMMEDIATE : (next_event - now);
		if(msg_rx_queue_get(usart_default_queue, &msg, &data, timeout)) {
			topo_dispatch(&msg, &data);
			msg_rx_queue_ack(usart_default_queue);
		}

		if (state == ANNOUNCE_CHILDREN && check_announcements_end()) {
			continue;
		}
	}

#ifdef TRACE
	if (topo_is_master) {
		console_printf("Topo finished:\n");
		for(size_t i = 0; i < topo_cell_count; i++) {
			const cell_info *c = &topo_cells[i];
			console_printf("(%02d, %02d) has id %d, dir = %d\n", c->pos.x, c->pos.y, c->id, c->dir);
		}
	}
#endif
	thread_t *thr = chThdCreateStatic(&route_thread_area, sizeof(route_thread_area), NORMALPRIO, route_thread_run, NULL);
	thr->name = "route";
}

static void transition_announce_children(void)
{
	if (state == FIND_SS) {
		state = ANNOUNCE_CHILDREN;
		if (!topo_is_master) {
			edges[topo_master_direction].type = EDGE_MASTER;
		} else {
#ifdef TRACE
			console_printf("I am master\n");
#endif
		}
		announce_not_a_child();
	}
}

static bool check_announcements_end(void)
{
	bool all_announced = true;
	for(enum direction d = 0; d < DIR_COUNT; d++) {
		edge_info *e = &edges[d];
		switch (e->type) {
		case EDGE_DEAD:
		case EDGE_NORMAL:
		case EDGE_MASTER:
			break;
		case EDGE_LIVE_UNKNOWN:
		case EDGE_CHILD:
			if (!e->announcements_end)
				all_announced = false;
		}
		continue;
	}

	if (all_announced) {
#ifdef TRACE
		console_printf("Got all announcements\n");
#endif
		sort_children();

		if (!topo_is_master) {
			announce_children();
		}
		state = ALLOCATE_IDS;
		if (topo_is_master) {
			send_ids();
			state = DONE;
		}
	}
	return all_announced;
}

static void sort_children(void)
{
	/* First find out where each direction would start */
	/* First slot is reserved for us */
	size_t start = 1;
	size_t wptr[DIR_COUNT];
	for(size_t d = 0; d < DIR_COUNT; d++) {
		wptr[d] = edges[d].first_cell = start;
		start += edges[d].children_count;
	}

	sort_cells[0] = topo_cells[0];
	for(size_t i = 1; i < topo_cell_count; i++) {
		enum direction d = topo_cells[i].dir;
		sort_cells[wptr[d]] = topo_cells[i];
		wptr[d]++;
	}
	memcpy(topo_cells, sort_cells, sizeof(cell_info) * topo_cell_count);

}

static void send_ids(void)
{
	/* Make sure that the cells have right numbering */
	for (size_t i = 0; i < topo_cell_count; i++) {
		topo_cells[i].id = first_cell_id + i;
	}
	for(size_t d = 0; d < DIR_COUNT; d++) {
		edge_info *e = &edges[d];
		if (e->type == EDGE_CHILD) {
			struct mgmt_alloc_ids msg;
			vector2 pos = pos_tx(topo_master_position, d);
			msg.first_id = e->first_cell + first_cell_id;
			msg.id_count = e->children_count;
			msg.master_x = pos.x;
			msg.master_y = pos.y;
			msg.master_dir = dir_tx(topo_master_orientation, d);
#ifdef TRACE
			console_printf("--> %d - %d, dir = %d\n", msg.first_id, msg.first_id + msg.id_count, d);
#endif
			usart_send_msg(d, TOPO_ALLOC_IDS, 0, sizeof(msg), &msg);
		}
	}
}

static void announce_not_a_child(void)
{
	for(size_t d = 0; d < DIR_COUNT; d++) {
		struct mgmt_not_a_child msg;
		edge_info *e = &edges[d];
		if (e->type != EDGE_MASTER && e->type != EDGE_DEAD) {
			usart_send_msg(d, TOPO_NOT_A_CHILD, 0, sizeof(msg), &msg);
		}
	}
}

static void announce_children(void)
{
	/* It is ok to block for the usart, since we do not expect any messages */
	struct mgmt_child_announce msg;
#ifdef TRACE
	console_printf("Announcing %u children\n", topo_cell_count);
#endif
	for(size_t i = 0; i < topo_cell_count; i++) {
		cell_info *c = &topo_cells[i];
		vector2 pos = pos_tx(c->pos, topo_master_direction);
		msg.x = pos.x;
		msg.y = pos.y;
		msg.final = false;
		usart_send_msg(master_usart, TOPO_CHILD_ANNOUNCE, 0, sizeof(msg), &msg);
	}
	msg.final = true;
	msg.x = 0;
	msg.y = 0;
	usart_send_msg(master_usart, TOPO_CHILD_ANNOUNCE, 0, sizeof(msg), &msg);
}

static void topo_dispatch(const msg_header *hdr, const buf_ptr *data)
{
	enum direction dir = hdr->source;
	edge_info *e = &edges[dir];
	if (e->type == EDGE_DEAD)
		e->type = EDGE_LIVE_UNKNOWN;

	switch(hdr->id) {
		case TOPO_ROUTE_ANNOUNCE: {
			struct mgmt_route_announce msg;
			if (state != FIND_SS)
				break;
			buf_ptr_read(data, 0, sizeof(msg), &msg);
			int cmp = memcmp(&msg.id.bytes, &master_cell_id.bytes, CELL_ID_LEN);
			if (cmp < 0 || (cmp == 0 && msg.distance < master_distance)) {
				topo_is_master = false;
				master_cell_id = msg.id;
				topo_master_direction = dir;
				master_usart = dir;
				master_distance = msg.distance;
				announce_route();
			}
		}; break;
		case TOPO_NOT_A_CHILD: {
			transition_announce_children();
			if (state != ANNOUNCE_CHILDREN)
				break;
			if (e->type == EDGE_LIVE_UNKNOWN)
				e->type = EDGE_NORMAL;
		}; break;
		case TOPO_CHILD_ANNOUNCE: {
			struct mgmt_child_announce msg;
			transition_announce_children();
			if (state != ANNOUNCE_CHILDREN)
				break;
			buf_ptr_read(data, 0, sizeof(msg), &msg);
			if (topo_cell_count < MAX_CELL_COUNT) {
				cell_info *c = &topo_cells[topo_cell_count++];
				c->pos.x = msg.x;
				c->pos.y = msg.y;
				c->pos = pos_rx(c->pos, dir);
				c->dir = dir;
				e->children_count++;
			}
			e->type = EDGE_CHILD;
			if (msg.final) {
#ifdef TRACE
				console_printf("Got final announcement from dir=%d\n", dir);
#endif
				e->announcements_end = true;
			}
		}; break;
		case TOPO_ALLOC_IDS: {
			struct mgmt_alloc_ids msg;
			if (state != ALLOCATE_IDS)
				break;
			buf_ptr_read(data, 0, sizeof(msg), &msg);
			first_cell_id = msg.first_id;
			topo_my_id = msg.first_id;
			topo_master_orientation = dir_rx(msg.master_dir, dir);
			topo_master_position = pos_rx(mk_vector2(msg.master_x, msg.master_y), dir);
#ifdef TRACE
			console_printf(
				"Allocated ids %d - %d. Master is at %d:%d.\n",
				first_cell_id, first_cell_id + msg.id_count - 1,
				topo_master.position.x, topo_master.position.y);
#endif
			send_ids();
			state = DONE;
		}; break;
	}
}

static void announce_route(void)
{
	for(enum direction d = 0; d < DIR_COUNT; d++) {
		struct mgmt_route_announce msg;
		msg.id = master_cell_id;
		msg.distance = master_distance + 1;
		usart_send_msg(d, TOPO_ROUTE_ANNOUNCE, MSG_DEFAULT, sizeof(msg), &msg);
	}
}

vector2 pos_tx(vector2 v, enum direction d) {
	int rot = d;
	return vector_rotate(v, -rot);
}

vector2 pos_rx(vector2 v, enum direction d) {
	/* Make the vector relative to us */
	v.y += 1;
	int rot = reverse_direction(d);
	/* And re-establish the top axis */
	return vector_rotate(v, rot);
}

enum direction dir_tx(enum direction value, enum direction d)
{
	return rotate_direction(value, -(int)d);
}

enum direction dir_rx(enum direction value, enum direction d)
{
	return rotate_direction(value, reverse_direction(d));
}

/************************************ ROUTING *****************************************************/
void route_message(bool send_local, msg_header *hdr, buf_ptr *buf)
{
	uint8_t route = (hdr->flags & MSG_ROUTE_MASK);
	uint8_t dir_mask = 0;
	bool local = false;
	if (route == MSG_ROUTE_MASTER) {
		if (topo_is_master) {
			local = true;
		} else {
			dir_mask = 1 << topo_master_direction;
		}
	} else if (route == MSG_ROUTE_UNICAST) {
		uint8_t recipient = *buf_ptr_index(buf, 0);
		if (recipient == topo_my_id) {
			local = true;
		} else if (recipient < first_cell_id  + 1 || recipient > first_cell_id + topo_cell_count) {
			console_printf(
				"topo_send_to: %d is not mine, I serve %d - %d\n",
				recipient, first_cell_id, first_cell_id + topo_cell_count);
		} else {
			const cell_info *c = &topo_cells[recipient - first_cell_id];
			dir_mask = 1 << c->dir;
		}
	} else if (route == MSG_ROUTE_BROADCAST) {
		for (enum direction d = 0; d < DIR_COUNT; d++) {
			if (edges[d].type == EDGE_CHILD)
				dir_mask |= (1 << d);
		}
		local = true;
	} else {
		return;
	}

	// console_printf("Routing mask 0x%x\n", dir_mask);
	for (enum direction d = 0; d < DIR_COUNT; d++) {
		if (((1 << d) & dir_mask) == 0)
			continue;
		usart_send_msg_buf(d, hdr->id, hdr->flags, hdr->length, buf);
	}

	if (local && send_local) {
		chSysLock();
		buf_ptr buf2;
		hdr->flags &= ~MSG_ROUTE_MASK;
		/* TODO: dispatch to correct queue */
		if(msg_rx_queue_reserveI(usart_default_queue, hdr, &buf2)) {
			buf_ptr_copy(&buf2, 0, buf, 0, hdr->length);
			msg_rx_queue_commitI(usart_default_queue, &buf2);
		}
		chSchRescheduleS();
		chSysUnlock();
	}
}

void send_routed_msg(bool send_local, uint8_t id, uint8_t flags, uint8_t len, void *data)
{
	msg_header hdr;
	hdr.id = id;
	hdr.flags = flags;
	hdr.length = len;
	hdr.source = 0xFF;
	/* Fake a buffer with our non-circular one */
	buf_ptr buf;
	buf.ptr = 0;
	buf.buffer = data;
	buf.size_bits = sizeof(qsize_t) * 8 - 1;
	route_message(send_local, &hdr, &buf);
}

static void route_thread_run(void *p)
{
	(void)p;
	while (1) {
		msg_header hdr;
		buf_ptr buf;
		if(msg_rx_queue_get(usart_route_queue, &hdr, &buf, TIME_INFINITE)) {
			route_message(true, &hdr, &buf);
			msg_rx_queue_ack(usart_route_queue);
		}
	}
}
