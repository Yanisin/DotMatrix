#include <hw/disp.h>
#include <applets/applet.h>
#include <util/icons.h>
#include <util/topo.h>
#include <util/message_queue.h>
#include <hw/usart_buffered.h>
#include <hw/buttons.h>
#include <hw/i2c.h>
#include "hw_defs.h"

#define PERIOD TIME_MS2I(75)
#define DUMMY_CYCLES 8

typedef struct img_ref_str {
	char *start;
	char *end;
} img_ref;

typedef struct img_desc_str {
	unsigned int w;
	unsigned int h;
	uint8_t *data;
} img_desc;

#define MSG_NEXT_ROW MSG_USER_ID(0)
#define MSG_CYCLE_IMAGE MSG_USER_ID(1)

#define EV_MSG 0
#define EV_BUTTON 1

extern const img_ref img_sysgo;
extern const img_ref img_banner;

static const img_ref * const imglist[] = {
	&img_sysgo,
	&img_banner,
};


static int current_img_id;
static unsigned int dummy_cycles;
static img_desc current_img;
static unsigned int scroll_column;

static void skip_comment(char ** ptr)
{
	if (**ptr == '#') {
		while (**ptr != '\n')
			(*ptr)++;
		(*ptr)++;
	}
}

static void parse_number(char** ptr, unsigned int *dst)
{
	*dst = 0;
	if (**ptr < '0' || **ptr > '9')
		chSysHalt("expected digit");
	while (**ptr >= '0' && **ptr <= '9') {
		*dst *= 10;
		*dst += (**ptr) - '0';
		(*ptr)++;
	}
}

/* is not immune to ill-formated images */
static void parse_pnm(const img_ref *ref, img_desc *d)
{
	char *ptr = (char*) ref->start;
	skip_comment(&ptr);
	if (ptr[0] != 'P' && ptr[1] != '5' && ptr[2] != '\n') {
		chSysHalt("Invalid image header");
	}
	ptr += 3;
	skip_comment(&ptr);
	parse_number(&ptr, &d->w);
	if (*ptr != ' ') {
		chSysHalt("expected space");
	}
	ptr += 1;
	parse_number(&ptr, &d->h);
	if (*ptr != '\n') {
		chSysHalt("expected space");
	}
	ptr += 1;
	/* just skip this */
	while (*ptr != '\n')
		ptr++;
	ptr++;
	d->data = (uint8_t*)ptr;

}

static void scroll_display(void)
{
	for (int col = 0; col < DISP_COLS_NUM - 1; col++) {
		for (int row = 0; row < DISP_ROWS_NUM; row++) {
			disp_set_noupdate(col, row, disp_get(col + 1, row));
		}
	}
}

static uint8_t img_get(const img_desc *desc, unsigned int x, unsigned int y)
{
	return desc->data[desc->w * y + x];
}

static void draw_first_row(void)
{
	for (int row = 0; row < DISP_ROWS_NUM; row++) {
		if (dummy_cycles) {
			disp_set_noupdate(DISP_COLS_NUM - 1, row, 0);
		} else {
			uint8_t v = img_get(&current_img, scroll_column, row)/8;
			disp_set_noupdate(DISP_COLS_NUM - 1, row, v);
		}
	}
}

static void scroller_send_last_row(void)
{
	enum direction global_left = rotate_direction(DIR_LEFT, topo_master_orientation);
	uint8_t last_row[DISP_ROWS_NUM];
	for (int row = 0; row < DISP_ROWS_NUM; row++) {
		last_row[row] = disp_get(0, row);
	}
	usart_send_msg(global_left, MSG_NEXT_ROW, MSG_DEFAULT, sizeof(last_row), last_row);
}

static void scroller_cycle(void)
{
	parse_pnm(imglist[current_img_id], &current_img);
	dummy_cycles = DUMMY_CYCLES;
	scroll_column = 0;
	if (current_img.h != 8) {
		chSysHalt("invalid image height");
	}
	current_img_id = (current_img_id + 1) % (sizeof(imglist)/sizeof(*imglist));
}

static void check_buttons(void)
{
	uint8_t button_events;
	if(button_get_any_events(&button_events, TIME_IMMEDIATE)) {
		if (button_events & BTN_EV_UP) {
			scroller_cycle();
			i2c_broadcast(MSG_CYCLE_IMAGE, 0, NULL);
		}
	}
}

static void scroller_master(void)
{
	msg_header hdr;
	buf_ptr buf;

	scroller_send_last_row();
	scroll_display();
	draw_first_row();
	disp_update_rows();

	if (dummy_cycles) {
		dummy_cycles--;
	} else {
		scroll_column++;
		if (scroll_column == current_img.w) {
			scroll_column = 0;
			dummy_cycles = DUMMY_CYCLES;
		}
	}
	if (msg_rx_queue_get(default_queue, &hdr, &buf, TIME_MS2I(200))) {
		if (hdr.id == MSG_CYCLE_IMAGE) {
			scroller_cycle();
		}
		msg_rx_queue_ack(default_queue);
	}
	check_buttons();
}

static void scroller_slave(void)
{
	msg_header hdr;
	buf_ptr buf;
	if (msg_rx_queue_get(default_queue, &hdr, &buf, PERIOD * 4)) {
		if (hdr.id == MSG_NEXT_ROW) {
			scroller_send_last_row();
			scroll_display();
			for (int row = 0; row < DISP_ROWS_NUM; row++) {
				disp_set_noupdate(DISP_COLS_NUM - 1, row, *buf_ptr_index(&buf, row));
			}
			disp_update_rows();
		} else if (hdr.id == MSG_CYCLE_IMAGE) {
			scroller_cycle();
		}
		msg_rx_queue_ack(default_queue);
	}
	check_buttons();
}

static void scroller_run(void)
{	
	disp_clean();
	disp_set_rotation(topo_master_orientation);
	enum direction global_right = rotate_direction(DIR_RIGHT, topo_master_orientation);
	if (topo_edges[global_right].type == EDGE_DEAD) {
		/* We are the first one, generate the data */
		scroller_cycle();
		while (!applet_should_end) {
			scroller_master();
		}
	} else {
		while(!applet_should_end) {
			scroller_slave();
		}
	}
}

struct applet scroller_applet = {
	.run = scroller_run,
	.icon = {
		ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
		ICON_ROW(0, 1, 1, 0, 1, 1, 1, 0),
		ICON_ROW(0, 0, 0, 0, 1, 0, 0, 0),
		ICON_ROW(0, 0, 0, 0, 1, 1, 1, 0),
		ICON_ROW(1, 1, 0, 0, 0, 0, 1, 0),
		ICON_ROW(0, 0, 0, 0, 0, 0, 1, 0),
		ICON_ROW(0, 1, 1, 0, 1, 1, 1, 0),
		ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	}
};
applet_add(scroller);
