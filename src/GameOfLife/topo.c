#include <string.h>
#include "topo.h"
#include "cell_id.h"
#include "ticker.h"
#include "mgmt_msg.h"
#include "console.h"
#include "usart_buffered.h"
#include "int.h"
#include "applet.h"


#define MAX_CELL_COUNT 64
#define FIND_SS_TIMEOUT 1000
#define REANNOUNCE 100
// #define TRACE

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
static bool topo_dispatch(const usart_header *hdr);
/* Transition to the announce children phase */
static void transition_announce_children(void);
static void send_ids(void);

/* Transition to ALLOCATE_IDS if appropriate */
static bool check_announcements_end(void);
/* Make sure the cells are consecutive w.r.t. direction in the cells array */
static void sort_children(void);
static void announce_children(void);

uint8_t topo_my_id;
bool topo_is_master;
cell_info topo_cells[MAX_CELL_COUNT];
size_t topo_cell_count;
enum direction topo_master_direction;

static edge_info edges[DIR_COUNT];
static cell_info sort_cells[MAX_CELL_COUNT];
static uint8_t first_cell_id;

static unsigned int topo_start_tick;
static unsigned int topo_next_announce;
static enum topo_state state;

static cell_id_t master_cell_id;
static uint8_t master_usart;
static uint8_t master_distance;

void topo_run(void)
{
	topo_is_master = true;
	master_cell_id = *get_cell_id();
	master_distance = 0;
	topo_next_announce = 0;
	state = FIND_SS;

	usart_buf_clear(0);
	usart_buf_clear(1);
	usart_buf_clear(2);
	usart_buf_clear(3);
	ticker_msleep(200);

	topo_start_tick = ticker_get_ticks();
#ifdef TRACE
	enum topo_state prev = DONE;
#endif

	while (state != DONE) {
		unsigned int now = ticker_get_ticks();
#ifdef TRACE
		if (prev != state) {
			console_printf("state %d, now = %u\n", state, now);
			prev = state;
		}
#endif
		if (state == FIND_SS && topo_next_announce <= now) {
			announce_route();
			topo_next_announce = now + REANNOUNCE;
		}

		if (state == FIND_SS && topo_start_tick + FIND_SS_TIMEOUT <= now) {
			transition_announce_children();
		}
		usart_recv_dispatch(topo_dispatch);
		if (state == ANNOUNCE_CHILDREN && check_announcements_end()) {
			continue;
		}
		worker_run_all();
		cpu_relax();
	}

	topo_my_id = first_cell_id + (topo_cell_count++);
	cell_info *me = &topo_cells[topo_my_id];
	me->dir = DIR_COUNT;
	me->pos = mk_vector2(0, 0);
	for (size_t i = 0; i < topo_cell_count; i++) {
		topo_cells[i].id = first_cell_id + i;
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
	size_t start = 0;
	size_t wptr[DIR_COUNT];
	for(size_t d = 0; d < DIR_COUNT; d++) {
		wptr[d] = edges[d].first_cell = start;
		start += edges[d].children_count;
	}

	for(size_t i = 0; i < topo_cell_count; i++) {
		enum direction d = topo_cells[i].dir;
		sort_cells[wptr[d]] = topo_cells[i];
		wptr[d]++;
	}
	memcpy(topo_cells, sort_cells, sizeof(cell_info) * topo_cell_count);

}

static void send_ids(void)
{
	for(size_t d = 0; d < DIR_COUNT; d++) {
		edge_info *e = &edges[d];
		if (e->type == EDGE_CHILD) {
			struct mgmt_alloc_ids msg;
			msg.first_id = e->first_cell + first_cell_id;
			msg.id_count = e->children_count;
#ifdef TRACE
			console_printf("--> %d - %d, dir = %d\n", msg.first_id, msg.first_id + msg.id_count, d);
#endif
			usart_send_msg(direction_to_usart(d), MGMT_ALLOC_IDS, sizeof(msg), &msg);
		}
	}
}

static void announce_not_a_child(void)
{
	for(size_t d = 0; d < DIR_COUNT; d++) {
		struct mgmt_not_a_child msg;
		edge_info *e = &edges[d];
		if (e->type != EDGE_MASTER && e->type != EDGE_DEAD) {
			usart_send_msg(direction_to_usart(d), MGMT_NOT_A_CHILD, sizeof(msg), &msg);
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
		usart_send_msg(master_usart, MGMT_CHILD_ANNOUNCE, sizeof(msg), &msg);
	}
	msg.final = true;
	msg.x = 0;
	msg.y = 0;
	usart_send_msg(master_usart, MGMT_CHILD_ANNOUNCE, sizeof(msg), &msg);
}

static bool topo_dispatch(const usart_header *hdr)
{
	enum direction dir = usart_to_direction(hdr->usart);
	edge_info *e = &edges[dir];
	if (e->type == EDGE_DEAD)
		e->type = EDGE_LIVE_UNKNOWN;

	switch(hdr->id) {
		case MGMT_ROUTE_ANNOUNCE: {
			struct mgmt_route_announce msg;
			if (state != FIND_SS)
				break;
			if (usart_recv_msg(hdr->usart, sizeof(msg), &msg)) {
				int cmp = memcmp(&msg.id.bytes, &master_cell_id.bytes, CELL_ID_LEN);
				if (cmp < 0 || (cmp == 0 && msg.distance < master_distance)) {
					topo_is_master = false;
					master_cell_id = msg.id;
					topo_master_direction = dir;
					master_usart = direction_to_usart(dir);
					master_distance = msg.distance;
					announce_route();
				}
			}
			return true;
		}
		case MGMT_NOT_A_CHILD: {
			struct mgmt_not_a_child msg;
			transition_announce_children();
			if (state != ANNOUNCE_CHILDREN)
				break;
			if (e->type == EDGE_LIVE_UNKNOWN)
				e->type = EDGE_NORMAL;
			usart_recv_msg(hdr->usart, sizeof(msg), &msg);

			return true;
		};
		case MGMT_CHILD_ANNOUNCE: {
			struct mgmt_child_announce msg;
			transition_announce_children();
			if (state != ANNOUNCE_CHILDREN)
				break;
			if (usart_recv_msg(hdr->usart, sizeof(msg), &msg)) {
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
			}
			return true;
		}
		case MGMT_ALLOC_IDS: {
			struct mgmt_alloc_ids msg;
			if (state != ALLOCATE_IDS)
				break;
			if (usart_recv_msg(hdr->usart, sizeof(msg), &msg)) {
#ifdef TRACE
				console_printf("Allocated ids %d - %d\n", msg.first_id, msg.first_id + msg.id_count - 1);
#endif
				first_cell_id = msg.first_id;
				send_ids();
				state = DONE;
			}
			return true;
		}
	}
	return false;
}

static void announce_route(void)
{
	for(enum direction d = 0; d < DIR_COUNT; d++) {
		struct mgmt_route_announce msg;
		msg.id = master_cell_id;
		msg.distance = master_distance + 1;
		usart_send_msg(direction_to_usart(d), MGMT_ROUTE_ANNOUNCE, sizeof(msg), &msg);
	}
}

bool topo_send_up(uint8_t msgid, uint8_t len, const void *data)
{
	if (topo_is_master)
		return true;
	usart_send_msg(master_usart, msgid, len, data);
	return false;
}

void topo_send_down(uint8_t msgid, uint8_t len, const void *data)
{
	for(enum direction d = 0; d < DIR_COUNT; d++) {
		if (edges[d].type == EDGE_CHILD)
			usart_send_msg(direction_to_usart(d), msgid, len, data);
	}
}

bool topo_send_to(uint8_t recipient, uint8_t msgid, uint8_t len, const void *data)
{
	if (recipient == topo_my_id)
		return true;
	if (recipient < first_cell_id || recipient >= first_cell_id + topo_cell_count) {
		console_printf(
			"topo_send_to: %d is not mine, I serve %d - %d\n",
			recipient, first_cell_id, first_cell_id + topo_cell_count);
		return false;
	}
	const cell_info *c = &topo_cells[recipient - first_cell_id];
	// console_printf("topo_send_to: id %d -> %d\n", recipient, c->dir);
	usart_send_msg(direction_to_usart(c->dir), msgid, len, data);
	return false;
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


