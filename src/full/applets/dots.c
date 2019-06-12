#include <stdint.h>
#include <util/rand.h>
#include <util/topo.h>
#include <util/message_queue.h>
#include <util/vector.h>
#include <util/icons.h>
#include <util/timeout.h>
#include <applets/applet.h>
#include <hw/i2c.h>
#include <hw/disp.h>
#include <hw_defs.h>

#define MAX_DOTS 16
#define DOT_ADD_TRESHOLD 4
#define FADE_PERIOD TIME_MS2I(25)
#define MOVE_PERIOD 8

#define MSG_MIGRATE_DOT MSG_USER_ID(0)

struct msg_mgirate_dot {
	uint8_t pos;
	uint8_t age;
};

typedef struct dot_s {
	uint8_t used;
	uint8_t x;
	uint8_t y;
	uint8_t direction;
	uint8_t age;
} dot_t;


static uint8_t dots_num;
static dot_t dots[MAX_DOTS];
static systime_t next_tick;
static int fade_periods;

static void dots_init(void)
{
	int i;
	for (i=0;i<MAX_DOTS;i++) {
		dots[i].used = 0;
	}
}

static void add_dot(void)
{
	int i;
	for (i = 0; i < MAX_DOTS; i++) {
		if(dots[i].used == 0) {
			break;
		}
	}
	dots[i].x = rand_get() % DISP_COLS_NUM;
	dots[i].y = rand_get() % DISP_ROWS_NUM;
	dots[i].direction = rand_get() % DIR_COUNT;
	dots[i].used = 1;
	dots[i].age = 0;
	dots_num++;
}

static void add_existing_dot(enum direction d, uint8_t x, uint8_t y, uint8_t age) {
	int i;
	if (dots_num >= MAX_DOTS)
		return;
	for (i = 0; i < MAX_DOTS; i++) {
		if(dots[i].used == 0) {
			break;
		}
	}

	dots[i].used = 1;
	dots[i].direction = d;
	dots[i].x = x;
	dots[i].y = y;
	dots[i].age = age;
	dots_num++;
}

static void age_dots(void) {
	for (int i = 0; i < MAX_DOTS; i++) {
		if (dots[i].age < 32)
			dots[i].age++;
	}
}

static void receive_new(void)
{
	msg_header hdr;
	buf_ptr data;
	if (msg_rx_queue_get(default_queue, &hdr, &data, timeout_until(next_tick))) {
		struct msg_mgirate_dot msg;
		buf_ptr_read(&data, 0, sizeof(msg), &msg);
		uint8_t v = msg.pos;
		switch (hdr.source) {
		case MSG_SOURCE_USART_UP:
			add_existing_dot(DIR_DOWN, DISP_ROWS_NUM - 1 - v, 0, msg.age);
			break;
		case MSG_SOURCE_USART_RIGHT:
			add_existing_dot(DIR_LEFT, DISP_COLS_NUM - 1, DISP_COLS_NUM - 1 - v, msg.age);
			break;
		case MSG_SOURCE_USART_DOWN:
			add_existing_dot(DIR_UP, v, DISP_ROWS_NUM - 1, msg.age);
			break;
		case MSG_SOURCE_USART_LEFT:
			add_existing_dot(DIR_RIGHT, 0, v, msg.age);
			break;
		}
		msg_rx_queue_ack(default_queue);
	}

}

static void show_dots(void)
{
	int i;
		/* draw */
	for (i=0;i<MAX_DOTS;i++) {
		if(dots[i].used != 0) {
			disp_set_noupdate(dots[i].x, dots[i].y, dots[i].age);
		}
	}
	disp_update_rows();
}

static void migrate_dot(int i, enum direction dir, uint8_t pos)
{
	struct msg_mgirate_dot msg;
	msg.pos = pos;
	msg.age = dots[i].age;
	usart_send_msg(dir, MSG_MIGRATE_DOT, MSG_DEFAULT, sizeof(msg), &msg);
	dots[i].used = 0;
	dots_num--;
}

static void move_dot(int i)
{
	/* We number dot positions on the edge clockwise, similar to GameOfLife */
	switch(dots[i].direction) {
	case DIR_LEFT:
		if(dots[i].x == 0) {
			migrate_dot(i, DIR_LEFT, DISP_ROWS_NUM - 1 - dots[i].y);
		} else {
			dots[i].x--;
		}
		break;
	case DIR_RIGHT:
		if(dots[i].x == DISP_COLS_NUM - 1) {
			migrate_dot(i, DIR_RIGHT, dots[i].y);
		} else {
			dots[i].x++;
		}
		break;
	case DIR_UP:
		if(dots[i].y == 0) {
			migrate_dot(i, DIR_UP, dots[i].x);
		} else {
			dots[i].y--;
		}
		break;
	case DIR_DOWN:
		if(dots[i].y == DISP_ROWS_NUM - 1) {
			migrate_dot(i, DIR_DOWN, DISP_COLS_NUM - 1 - dots[i].x);
		} else {
			dots[i].y++;
		}
		break;
	}
}

static void move_dots(void)
{
	int i;
	/* recalculate */

	for (i = 0; i < MAX_DOTS; i++) {
		if(dots[i].used != 0) {
			move_dot(i);
		}
	}

}

static void dots_tick(void)
{
	/* The idea of the main loop here is that we are waiting for the next tick or any
	 * message to arrive. If a message arrives (receive_new), we redraw the dots.
	 */
	if (chVTGetSystemTimeX() >= next_tick) {
		next_tick = next_tick + FADE_PERIOD;
		fade_periods++;
		age_dots();
		if (fade_periods == MOVE_PERIOD - 1) {
			fade_periods = 0;
			move_dots();
		}
	}
	disp_clean();
	show_dots();
	/* Blocks until message arives or next_tick is here */
	receive_new();
	if(dots_num < DOT_ADD_TRESHOLD) {
		add_dot();
	}
}

static void dots_worker(void)
{
	dots_init();
	disp_clean();
	next_tick = chVTGetSystemTimeX();
	while(!applet_should_end) {
		dots_tick();
	}
}

static const struct applet dots_applet = {
	.run = dots_worker,
	.icon = {
		ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
		ICON_ROW(0, 1, 0, 0, 0, 0, 0, 0),
		ICON_ROW(0, 0, 0, 0, 0, 1, 0, 0),
		ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
		ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
		ICON_ROW(0, 0, 0, 1, 0, 0, 0, 0),
		ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
		ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	}
};
applet_add(dots);




