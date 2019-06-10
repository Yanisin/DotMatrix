#include <stdint.h>


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "hw_defs.h"
#include "applet.h"
#include "disp.h"
#include "ticker.h"
#include "dots.h"
#include "rand.h"
#include "usart_buffered.h"
#include "cdcacm.h"
#include "led.h"


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
#define DISP_TEST_DLY 80
static void disp_test(void)
{
	disp_clean();
	int i;
	for (i=0 ;i<4;i++) {
		disp_set(i,i, 31);
		ticker_msleep(DISP_TEST_DLY);
		if(i>0){
			disp_set(i-1,i-1, 0);
		}
		disp_set(i,7-i, 31);
		ticker_msleep(DISP_TEST_DLY);
		if(i>0) {
			disp_set(i-1,7-i+1, 0);
		}
		disp_set(7-i,7-i, 31);
		ticker_msleep(DISP_TEST_DLY);
		if (i>0) {
			disp_set(7-i+1,7-i+1, 0);
		}
		disp_set(7-i,i, 31);
		ticker_msleep(DISP_TEST_DLY);
		if (i>0) {
			disp_set(7-i+1,i-1, 0);
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


int main(void)
{
	int i=0;
	/* we want 48 MHz sysclk */
	rcc_clock_setup_in_hsi_out_48mhz();
	usart_init();
	led_init();
	ticker_init();
	disp_init();
	rand_init();

//	usart_send(USART_DIR_UP,'U');
//	usart_send(USART_DIR_DOWN,'D');
//	usart_send(USART_DIR_LEFT,'L');
//	usart_send(USART_DIR_RIGHT,'R');

	applet_init_all();
	disp_test();

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
	
	while (1) {
		applet_run_all();
		cpu_relax();
	}
	

}
