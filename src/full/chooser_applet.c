#include "usart_buffered.h"
#include "applet.h"
#include "disp.h"
#include "icons.h"
#include "topo.h"
#include "console.h"
#include "buttons.h"
#include "i2c.h"

#define MSG_SHOW_APPLET MSG_USER_ID(0)
struct msg_show_applet {
	uint8_t cell_id;
	uint8_t applet;
	uint8_t selected;
};

#define MSG_SELECT MSG_USER_ID(1)
#define MSG_CYCLE MSG_USER_ID(2)

static size_t page;
static size_t selected;
static bool activity;

static void chooser_show(struct msg_show_applet *msg);
static void chooser_master_update(void);

static void chooser_master_update(void) {
	page = selected / topo_cell_count;
	for(size_t i = 0; i < topo_cell_count; i++) {
		struct msg_show_applet msg;
		msg.cell_id = i;
		msg.applet = page * topo_cell_count + i;
		msg.selected = msg.applet == selected;
		if (topo_cell_count == 1)
			msg.selected = false;

		if (i == topo_my_id)
			chooser_show(&msg);
		else
			i2c_send(msg.cell_id, MSG_SHOW_APPLET, sizeof(msg), &msg);
	}
}

static void chooser_select(void)
{
	applet_select(selected);
}

static void chooser_cycle(void)
{
	activity = true;
	selected += 1;
	selected %= applet_count();
	chooser_master_update();
}

static void chooser_show(struct msg_show_applet *msg)
{
	const struct applet *a;
	activity = true;
	console_printf("showing %d\n", msg->applet);
	if (msg->applet >= applet_count()) {
		disp_clean();
		return;
	}
	a = applet_get(msg->applet);
	draw_icon(a->icon, msg->selected ? BLIT_NOT : BLIT_SET);
}

static void read_messages(void)
{
	msg_header hdr;
	buf_ptr data;

	while (msg_rx_queue_get(default_queue, &hdr, &data, TIME_IMMEDIATE)) {
		if (hdr.id == MSG_SHOW_APPLET) {
			struct msg_show_applet msg;
			buf_ptr_read(&data, 0, sizeof(msg), &msg);
			chooser_show(&msg);
		} else if (hdr.id == MSG_SELECT) {
			chooser_select();
		} else if (hdr.id == MSG_CYCLE) {
			chooser_cycle();
		}
		msg_rx_queue_ack(default_queue);
	}
}

static void chooser_run(void)
{
	event_listener_t usart_events;
	chEvtRegister(&default_queue->event, &usart_events, 0);
	event_listener_t applet_end;
	chEvtRegister(&applet_should_end_event, &applet_end, 1);
	event_listener_t buttons;
	chEvtRegister(&button_events_ready, &buttons, 2);

	eventmask_t wait = EVENT_MASK(0) | EVENT_MASK(1) | EVENT_MASK(2);
	eventmask_t evt = wait;
	uint8_t button_event_mask;
	activity = false;

	if (topo_is_master)
		chooser_master_update();

	while (!applet_should_end) {
		if (evt == 0 && topo_is_master) {
			/* Nothing happened pressed before a timeout */
			applet_select(applet_topmost());
		}
		if (evt & EVENT_MASK(0)) {
			read_messages();
		}
		if ((evt & EVENT_MASK(2)) && button_get_any_events(&button_event_mask, TIME_IMMEDIATE)) {
			if (button_event_mask & BTN_EV_UP) {
				if (topo_is_master)
					chooser_cycle();
				else
					i2c_send(MASTER_CELL_ID, MSG_CYCLE, 0, NULL);
			}
			if (button_event_mask & BTN_EV_HOLD) {
				if (topo_is_master)
					chooser_select();
				else
					i2c_send(MASTER_CELL_ID, MSG_SELECT, 0, NULL);
			}
		}
		evt = chEvtWaitAnyTimeout(wait, activity ? TIME_INFINITE : TIME_S2I(5));
	}

	chEvtUnregister(&default_queue->event, &usart_events);
	chEvtUnregister(&applet_should_end_event, &applet_end);
	chEvtUnregister(&button_events_ready, &buttons);
}

const struct applet chooser_applet = {
	.run = chooser_run
};

