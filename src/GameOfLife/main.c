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
	int i,j,pos;
	
	if (num > 8) {
		pos = 17-num;
	} else {
		pos = num;
	}

	for (i=0;i<8;i++) {
		for (j=0;j<8;j++) {
			disp_set(j,i,bright[pos]*smileys[0][i][j]);
		}
	}
}

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

void common_main(void);

void common_main(void)
{
	int i=0;
	usart_init();
	led_init();
	ticker_init();
	disp_init();
	rand_init();
	common_gpio_init();

//	usart_send(USART_DIR_UP,'U');
//	usart_send(USART_DIR_DOWN,'D');
//	usart_send(USART_DIR_LEFT,'L');
//	usart_send(USART_DIR_RIGHT,'R');

	led_on();
	console_puts("Starting...\n");

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
		print_smile(i);
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

	if (applet_count() > 0) {
		applet_select(applet_get(0));
	}
	
	while (1) {
		if (applet_current()) {
			applet_current()->worker();
		}
		usart_recv_dispatch();
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
