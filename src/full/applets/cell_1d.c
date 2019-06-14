#include "hw_defs.h"
#include <hw/disp.h>
#include <applets/applet.h>
#include <util/topo.h>
#include <ch.h>
#include <hw/i2c.h>
#include <util/message_queue.h>
#include <util/rand.h>
#include <util/icons.h>
#include <string.h>

#define PERIOD TIME_MS2I(200)
#define MSG_TICK MSG_USER_ID(0)
#define MSG_NEIGHBOR MSG_USER_ID(1)
#define MSG_SEND_ROW MSG_USER_ID(2)
#define MSG_NEW_RULE MSG_USER_ID(3)


static bool first_line[DISP_COLS_NUM + 2];
static bool next_line[DISP_COLS_NUM + 2];

#define FL_UP_NEIGHBOR 0
#define FL_FIRST 1
#define FL_LAST DISP_ROWS_NUM
#define FL_DOWN_NEIGHBOR (DISP_ROWS_NUM + 1)

#define CHANGE_GENERATIONS 50
static uint8_t rule = 18;
static int generations = 0;

/*
 *   top
 *    ^
 *   +----+
 *   |    |   -> scroll direction
 *   |    |
 *   +----+
 *    bottom
 *
 * The first line is numbered the top, where 0 = neighbors cell value.
 */

static void scroll_display(void)
{
	for (int col = DISP_COLS_NUM - 1; col > 0; col--) {
		for (int row = 0; row < DISP_ROWS_NUM; row++) {
			disp_set_noupdate(col, row, disp_get(col - 1, row));
		}
	}
}

static void seed_first_line_with_random(void)
{
	for (int col = FL_FIRST; col <= FL_LAST + 1; col++) {
		first_line[col] = rand_get() % 2;
	}
}

static void cell_1d_recv_neihgbor(void)
{
	msg_header hdr;
	buf_ptr data;
	while(msg_rx_queue_get(default_queue, &hdr, &data, TIME_IMMEDIATE)) {
		if (hdr.id == MSG_NEIGHBOR) {
			if (hdr.source == MSG_SOURCE_USART_UP) {
				first_line[FL_UP_NEIGHBOR] = *buf_ptr_index(&data, 0);
			} else if (hdr.source == MSG_SOURCE_USART_DOWN) {
				first_line[FL_DOWN_NEIGHBOR] = *buf_ptr_index(&data, 0);
			}
		} else if (hdr.id == MSG_NEW_RULE) {
			rule = *buf_ptr_index(&data, 0);
			seed_first_line_with_random();
		}
		msg_rx_queue_ack(default_queue);
	}
}

static void cell_1d_update(void)
{
	for(int i = FL_FIRST; i <= FL_LAST; i++) {
		uint8_t id = ((first_line[i - 1]) << 2)
			| ((first_line[i]) << 1)
			| ((first_line[i + 1]) << 0);
		next_line[i] = !!((rule) & (1 << id));
	}
	memcpy(first_line, next_line, sizeof(first_line));
	scroll_display();
	for(int i = FL_FIRST; i <= FL_LAST; i++) {
		disp_set_noupdate(0, i - 1, first_line[i] ? 31 : 0);
	}
}

static void cell_1d_send_neighbor(void)
{
	usart_send_msg(DIR_UP, MSG_NEIGHBOR, MSG_DEFAULT, 1, &first_line[FL_FIRST]);
	usart_send_msg(DIR_DOWN, MSG_NEIGHBOR, MSG_DEFAULT, 1, &first_line[FL_LAST]);
}

static void cell_1d_send_scroll(void)
{
	enum direction global_right = rotate_direction(DIR_RIGHT, topo_master_orientation);
	uint8_t last_row[DISP_ROWS_NUM];
	for (int row = 0; row < DISP_ROWS_NUM; row++) {
		last_row[row] = disp_get(DISP_COLS_NUM - 1, row);
	}
	usart_send_msg(global_right, MSG_SEND_ROW, MSG_DEFAULT, sizeof(last_row), last_row);
}

static void cell_1d_recv_scroll(void)
{
	msg_header hdr;
	buf_ptr data;
	while(msg_rx_queue_get(default_queue, &hdr, &data, TIME_IMMEDIATE)) {
		if (hdr.id == MSG_SEND_ROW) {
			for (int row = 0; row < DISP_ROWS_NUM; row++) {
				disp_set_noupdate(0, row, *buf_ptr_index(&data, row));
			}
		}
		msg_rx_queue_ack(default_queue);
	}
}

static void cell_1d_run(void)
{
	disp_clean();
	disp_set_rotation(topo_master_orientation);
	enum direction global_left = rotate_direction(DIR_LEFT, topo_master_orientation);
	bool is_generator = topo_edges[global_left].type == EDGE_DEAD;
	if (is_generator) {
		seed_first_line_with_random();
		cell_1d_send_neighbor();
	}
	while (!applet_should_end) {
		/* Wait for tick or generate it */
		if (topo_is_master) {
			generations++;
			if ((generations % CHANGE_GENERATIONS) == 0 && generations != 0) {
				rule = rand_get() & 0xFF;
				seed_first_line_with_random();
				i2c_broadcast(MSG_NEW_RULE, 1, &rule);
			}
			chThdSleep(PERIOD);
			i2c_broadcast(MSG_TICK, 0, NULL);
		} else {
			msg_header header;
			buf_ptr data;
			if (msg_rx_queue_get(alt_queue, &header, &data, TIME_INFINITE)) {
				msg_rx_queue_ack(alt_queue);
			}
		}

		if (is_generator) {
			/* We are the first cell, generate the cells */
			cell_1d_recv_neihgbor();
			cell_1d_update();
			cell_1d_send_neighbor();
			cell_1d_send_scroll();
		} else {
			scroll_display();
			cell_1d_recv_scroll();
			cell_1d_send_scroll();
		}
		disp_update_rows();
	}
}

static msg_rx_queue *cell_1d_dispatch_msg(const struct msg_header_str *h)
{
	if (h->id == MSG_TICK) {
		return alt_queue;
	} else {
		return default_queue;
	}
}

static const struct applet cell_1d_applet = {
	.run = cell_1d_run,
	.dispatch_msg = cell_1d_dispatch_msg,
	.icon = {
		ICON_ROW(0, 0, 0, 0, 1, 0, 0, 0),
		ICON_ROW(0, 0, 0, 1, 0, 1, 0, 0),
		ICON_ROW(0, 0, 1, 0, 0, 0, 1, 0),
		ICON_ROW(0, 1, 0, 1, 0, 1, 0, 1),
		ICON_ROW(1, 0, 0, 0, 0, 0, 0, 0),
		ICON_ROW(0, 1, 0, 0, 0, 0, 0, 1),
		ICON_ROW(0, 0, 1, 0, 0, 0, 1, 0),
		ICON_ROW(0, 1, 0, 1, 0, 1, 0, 1),
	}
};

applet_add(cell_1d);
