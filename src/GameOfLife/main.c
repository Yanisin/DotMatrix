#include <stdint.h>


#ifndef SIM
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include "hw_defs.h"
#endif

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


uint8_t bright[]={1,2,3,5,7,10,14, 20, 31};

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

#if 0
static const uint8_t grey [1][8][8] = {
	{{ 0, 1, 2, 3, 4 ,5, 6, 7},
	 { 1, 2, 3, 4, 5, 6, 7, 8},
	 { 2, 3, 4, 5, 6, 7, 8, 9},
	 { 3, 4, 5, 6, 7, 8, 9,11},
	 { 4, 5, 6, 7, 8, 9,11,16},
	 { 5, 6, 7, 8, 9,11,16,20},
	 { 6, 7, 8, 9,11,16,20,27},
	 { 7, 8, 9,11,16,20,27,31}
	 }};
#endif


#if 0
static const uint8_t digits[6][5][5] = {
	{{0,0,255,255,0},
	{0,255,0,0,255},
	{0,255,0,0,255},
	{0,255,0,0,255},
	{0,0,255,255,0}},

	{{0,0,0,255,0},
	{0,0,255,255,0},
	{0,0,0,255,0},
	{0,0,0,255,0},
	{0,0,255,255,255}},

	{{0,0,255,255,0},
	 {0,255,0,0,255},
	 {0,0,0,255,0},
	 {0,0,255,0,0},
	 {0,255,255,255,255}},


	{{0,0,255,255,0},
	 {0,255,0,0,255},
	 {0,0,0,255,0},
	 {0,255,0,0,255},
	 {0,0,255,255,0}},

	{{0,0,0,255,0},
	 {0,0,255,255,0},
	 {0,255,0,255,0},
	 {0,255,255,255,255},
	 {0,0,0,255,0}},


	{{0,255,255,255,255},
	 {0,255,0,0,0},
	 {0,255,255,255,0},
	 {0,0,0,0,255},
	 {0,255,255,255,0}}

};
#endif

static void print_smile(uint8_t num)
{
	int i,j;

	for (i=0;i<8;i++) {
		for (j=0;j<8;j++) {
			disp_set(j,i, 31*smileys[0][i][j]);
		}
	}
}

#if 0
static void print_grey(uint8_t num)
{
	int i,j;

	for (i=0;i<8;i++) {
		for (j=0;j<8;j++) {
			switch(num){
				case 0:
				disp_set(j,i,grey[0][i][j]);
				break;
				case 1:
				disp_set(7-j,i,grey[0][i][j]);
				break;
				case 2:
				disp_set(7-j,7-i,grey[0][i][j]);
				break;
				case 3:
				disp_set(j,7-i,grey[0][i][j]);
				break;
			}
		}
	}
}
#endif

extern const struct applet topo_applet;

static bool dispatch_usart(const usart_header* hdr)
{
	if (hdr->id & MSG_ID_FLAG_MGMT) {
		/* Not handled yet */
	} else {
		const struct applet * a = applet_current();
		if (a && a->check_usart) {
			return a->check_usart(hdr);
		}
	}
	return false;
}


void common_main(void);

void common_main(void)
{
	usart_init();
	led_init();
	ticker_init();
	disp_init();
	rand_init();
	common_gpio_init();

	console_puts("Starting...\n");
	led_on();
	print_smile(0);


#if 0
	disp_set(0, 0, 31);
	ticker_msleep(200);
	led_off();
	disp_set(7, 0, 31);
	ticker_msleep(200);
	led_on();
	disp_set(7, 7, 31);
	ticker_msleep(200);
	led_off();
	disp_set(0, 7, 31);
	ticker_msleep(200);
	disp_clean();

	for ( i = 0; i < 18; i++) {

		ticker_msleep(100);
	}

	for ( i = 0; i < 4; i++) {
		print_grey(i);
		led_on();
		ticker_msleep(200);
		led_off();
		ticker_msleep(200);
	}

	disp_clean();

	led_on();
#endif
	topo_run();

	applet_select(&topo_applet);
	
	while (1) {
		if (applet_current()) {
			applet_current()->worker();
		}
		usart_recv_dispatch(dispatch_usart);
		cpu_relax();
	}
	

}

#ifndef SIM
int main(void) {
	/* we want 48 MHz sysclk */
	rcc_clock_setup_in_hsi_out_48mhz();
	common_main();
}
#endif
