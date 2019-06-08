#include <ch.h>
#include "board.h"
#include "led.h"

#include "applet.h"
#include "disp.h"
#include "dots.h"
#include "rand.h"
#include "usart_buffered.h"
#include "cdcacm.h"
#include "led.h"
#include "console.h"
#include "common_gpio.h"
#include "topo.h"
#include "message_queue.h"
#include "mgmt_proto.h"
#include "icons.h"

#ifdef SIM
#include "sim/main.h"
#endif

static void main_task(void);

#ifdef SIM
int main(int argc, char* argv[])
{
	int ret = sim_main(argc, argv);
	if (ret != 0)
		return ret;
	chSysInit();
	main_task();
	return 0;
}
#else
int main(void)
{
	board_init();
	chSysInit();
	main_task();
	return 0;
}
#endif

extern const struct applet chooser_applet;
static THD_WORKING_AREA(mgmt_thread_area, 128);

static void handle_bl_start(void)
{
#ifndef SIM
	scb_reset_system();
#endif
}

static void handle_mgmt_interrupt(void)
{
	// nothing yet
}

static void handle_change_applet(const buf_ptr *data)
{
	struct mgmt_change_applet msg;
	buf_ptr_read(data, 0, sizeof(msg), &msg);
	// console_printf("Change applet %d\n", msg.applet);
	applet_select_local(applet_get(msg.applet));
}

static void mgmt_task(void *p)
{
	(void)p;
	while (1) {
		msg_header hdr;
		buf_ptr data;
		if (msg_rx_queue_get(usart_mgmt_queue, &hdr, &data, TIME_INFINITE)) {
			switch (hdr.id) {
				case MGMT_BL_START:
					handle_bl_start();
					break;
				case MGMT_INTERRTUP:
					handle_mgmt_interrupt();
					break;
				case MGMT_CHANGE_APPLET:
					route_message(false, &hdr, &data);
					handle_change_applet(&data);
					break;
			}
			msg_rx_queue_ack(usart_mgmt_queue);
		}
	}
}

static void main_task(void)
{
	led_init();
	disp_init();
	rand_init();
	common_gpio_init();
	led_on();

	draw_icon(smiley, false);
	worker_init_all();
	thread_t *thr = chThdCreateStatic(mgmt_thread_area, sizeof(mgmt_thread_area), NORMALPRIO + 2, mgmt_task, NULL);
	thr->name = "mgmt";

	console_puts("Starting...\n");
	topo_run();

	if (topo_is_master)
		chThdSleep(TIME_MS2I(50));

	applet_select_local(&chooser_applet);

	while (1) {
		applet_should_end = false;
		applet_current()->run();
		if (!applet_should_end) {
			chSysHalt("Applet ended unexpectedly");
		}
	}
}
