#include <ch.h>
#include <hw/board.h>
#include <hw/led.h>
#include <hw/disp.h>
#include <hw/usart_buffered.h>
#include <hw/cdcacm.h>
#include <hw/led.h>
#include <hw/console.h>
#include <hw/i2c.h>
#include <hw/cell_id.h>
#include <util/topo.h>
#include <util/message_queue.h>
#include <util/mgmt_proto.h>
#include <util/icons.h>
#include <util/topo.h>
#include <util/initializers.h>
#include <applets/applet.h>
#include <assert.h>

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
	board_init();
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

static THD_WORKING_AREA(mgmt_thread_area, 128);
static void mgmt_task(void *p)
{
	(void)p;
	while (1) {
		msg_header hdr;
		buf_ptr data;
		if (msg_rx_queue_get(mgmt_queue, &hdr, &data, TIME_INFINITE)) {
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
			msg_rx_queue_ack(mgmt_queue);
		}
	}
}

static THD_WORKING_AREA(disp_test_thread_area, 128);
#define DISP_TEST_DLY TIME_MS2I(100)
static void disp_test_task(void *p)
{
	(void)p;

	disp_clean();
	int i;
	for (i=0 ;i<4;i++) {
		if (chThdShouldTerminateX())
			return;
		disp_set(i,i, 31);
		chThdSleep(DISP_TEST_DLY);
		if(i>0){
			disp_set(i-1,i-1, 0);
		}

		disp_set(i,7-i, 31);
		chThdSleep(DISP_TEST_DLY);
		if(i>0) {
			disp_set(i-1,7-i+1, 0);
		}

		disp_set(7-i,7-i, 31);
		chThdSleep(DISP_TEST_DLY);
		if (i>0) {
			disp_set(7-i+1,7-i+1, 0);
		}

		disp_set(7-i,i, 31);
		chThdSleep(DISP_TEST_DLY);
		if (i>0) {
			disp_set(7-i+1,i-1, 0);
		}
	}
}

static char hexdigit(uint8_t x) {
	return (x < 10 ? '0' + x : 'A' + x - 10);
}

static void print_banner(void)
{
	char cell_id_hex[CELL_ID_LEN*2 + 1];
	cell_id_hex[CELL_ID_LEN*2] = 0;
	const cell_id_t *id = get_cell_id();
	for(int i = 0; i < CELL_ID_LEN; i++) {
		cell_id_hex[i*2] = hexdigit(id->bytes[i] >> 4);
		cell_id_hex[i*2 + 1] = hexdigit(id->bytes[i] & 0xF);
	}
	console_printf("\nWelcome, this is DotMatrix board ID %s\n", cell_id_hex);
	console_printf("Assigned cell id: %d, master=%s\n",
		topo_my_id,
		topo_is_master ? "true": "false");
}

static void main_task(void)
{
	disp_init();
	led_on();

	run_all_initializers();
	thread_t *thr = chThdCreateStatic(mgmt_thread_area, sizeof(mgmt_thread_area), NORMALPRIO + 2, mgmt_task, NULL);
	thr->name = "mgmt";
	display_test = chThdCreateStatic(disp_test_thread_area, sizeof(disp_test_thread_area), NORMALPRIO, disp_test_task, NULL);
	display_test->name = "disp";

	topo_run();
	/* I2C needs the topology */
	i2c_init();
	chThdWait(display_test);
	print_banner();

	const struct applet* autostart = applet_autostart();
	if (autostart == NULL)
		applet_select_local(&chooser_applet);
	else
		applet_select_local(autostart);

	while (1) {
		if (topo_is_master) {
			/* Give others some time to start the applet */
			/* TODO: add some "barrier" mechanism for these instances */
#ifdef SIM
	chThdSleep(TIME_MS2I(300));
#else
	chThdSleep(TIME_MS2I(100));
#endif
		}
		applet_should_end = false;
		applet_current()->run();
		if (!applet_should_end) {
			chSysHalt("Applet ended unexpectedly");
		}
	}
}
