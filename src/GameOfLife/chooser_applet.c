#include "usart_buffered.h"
#include "applet.h"
#include "disp.h"
#include "icons.h"
#include "topo.h"
#include "console.h"
#include "buttons.h"

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

static void chooser_show(struct msg_show_applet *msg);
static void chooser_update(void);

static void chooser_init(void) {
	if (!topo_is_master)
		return;
	chooser_update();
}

static void chooser_update(void) {
	page = selected / topo_cell_count;
	for(size_t i = 0; i < topo_cell_count; i++) {
		struct msg_show_applet msg;
		msg.cell_id = i;
		msg.applet = page * topo_cell_count + i;
		msg.selected = msg.applet == selected;
		if (topo_send_to(msg.cell_id, MSG_SHOW_APPLET, sizeof(msg), &msg)) {
			chooser_show(&msg);
		}
	}
}

static void chooser_select(void)
{
	applet_select(selected);
}

static void chooser_cycle(void)
{
	selected += 1;
	selected %= applet_count();
	chooser_update();
}

static void chooser_show(struct msg_show_applet *msg)
{
	const struct applet *a;
	console_printf("showing %d\n", msg->applet);
	if (msg->applet >= applet_count()) {
		disp_clean();
		return;
	}
	a = applet_get(msg->applet);
	draw_icon(a->icon, msg->selected);
}

static bool chooser_usart(const usart_header *hdr)
{
	if (hdr->id == MSG_SHOW_APPLET) {
		struct msg_show_applet msg;
		if (usart_recv_msg(hdr->usart, sizeof(msg), &msg)) {
			if (topo_send_to(msg.cell_id, MSG_SHOW_APPLET, sizeof(msg), &msg)) {
				chooser_show(&msg);
			}
		}
		return true;
	} else if (hdr->id == MSG_SELECT) {
		if(usart_recv_msg(hdr->usart, 0, NULL)) {
			if(topo_send_up(MSG_SELECT, 0, NULL)) {
				chooser_select();
			}
		}
	} else if (hdr->id == MSG_CYCLE) {
		if(usart_recv_msg(hdr->usart, 0, NULL)) {
			if(topo_send_up(MSG_CYCLE, 0, NULL)) {
				chooser_cycle();
			}
		}
	}
	return false;
}

static void chooser_worker(void)
{
	switch(button_get_event(BTN_ANY)) {
	case BTN_EV_UP:
		if(topo_send_up(MSG_CYCLE, 0, NULL)) {
			chooser_cycle();
		}
		break;
	case BTN_EV_HOLD:
		if(topo_send_up(MSG_SELECT, 0, NULL)) {
			chooser_select();
		}
		break;
	default:
		;
	}
}

const struct applet chooser_applet = {
	.init = chooser_init,
	.worker = chooser_worker,
	.check_usart = chooser_usart
};

