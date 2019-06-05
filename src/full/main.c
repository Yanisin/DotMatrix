#include <ch.h>
#include "board.h"
#include "led.h"

#include "applet.h"
#include "disp.h"
#include "ticker.h"
#include "dots.h"
#include "rand.h"
#include "usart_buffered.h"
#include "cdcacm.h"
#include "led.h"
#include "console.h"
#include "common_gpio.h"
#include "int.h"
#include "topo.h"
#include "mgmt_msg.h"

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

static const uint8_t smileys [1][8][8] = {
	{{ 0, 0, 1, 1, 1, 1, 0, 0},
	 { 0, 1, 0, 0, 0, 0, 1, 0},
	 { 1, 0, 1, 0, 0, 1, 0, 1},
	 { 1, 0, 1, 0,0,  1, 0, 1},
	 { 1, 1, 0, 0, 0, 0, 1, 1},
	 { 1, 0, 1, 1, 1, 1, 0, 1},
	 { 0, 1, 0, 0, 0, 0, 1, 0},
	 { 0, 0, 1, 1, 1, 1, 0, 0}
	 }};

static bool dispatch_usart(const usart_header* hdr)
{
	if (hdr->id & MSG_ID_FLAG_MGMT) {
		if(hdr->id == MGMT_CHANGE_APPLET) {
			struct mgmt_change_applet msg;
			if (usart_recv_msg(hdr->usart, sizeof(msg), &msg)) {
				applet_select(msg.applet);
			}
		}
	} else {
		const struct applet * a = applet_current();
		if (a && a->check_usart) {
			return a->check_usart(hdr);
		}
	}
	return false;
}

static void print_smile(void)
{
	int i,j;

	for (i=0;i<8;i++) {
		for (j=0;j<8;j++) {
			disp_set(j,i, 31*smileys[0][i][j]);
		}
	}
}


static void main_task(void)
{
	usart_init();
	led_init();
	ticker_init();
	disp_init();
	rand_init();
	common_gpio_init();
	led_on();
	print_smile();

	worker_init_all();
	console_puts("Starting...\n");
	topo_run();

	if (topo_is_master)
		ticker_msleep(150);

	applet_select_local(&chooser_applet);

	while (1) {
		if (applet_current()) {
			if (applet_current()->worker)
				applet_current()->worker();
		}
		usart_recv_dispatch(dispatch_usart);
		worker_run_all();
		cpu_relax();
	}
}
