#include "stdint.h"


#ifdef STDPERIPH
#include "stm32f0xx.h"
#else

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#endif

#include "hw_defs.h"
#include "disp.h"
#include "ticker.h"
#include "sled.h"
#include "jstick.h"
#include "usart_buffered.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#ifdef STDPERIPH
GPIO_InitTypeDef GPIO_InitStructure;
#endif


uint8_t bright[]={1,2,3,5,7,10,14, 20, 32};

static const uint8_t smileys [1][8][8] = {
	{{ 0, 0, 1, 1, 1, 1, 0, 0},
	 { 0, 1, 0, 0, 0, 0, 1, 0},
	 { 1, 0, 1, 0, 0, 1, 0, 1},
	 { 1, 0, 1, 0,0,  1, 0, 1},
	 { 1, 1, 0, 0, 0, 0, 1, 1},
	 { 1, 0, 1, 1, 1, 1, 0,1},
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
	 { 7, 8, 9,11,16,20,27,32}
	 }};



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


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static void print_smile(uint8_t num)
{
	int i,j,pos;
	
	if (num > 8) {
		pos = 17-num;
	}else{
		pos = num;
	}


	for (i=0;i<8;i++) {
		for (j=0;j<8;j++) {
			disp_set(j,i,bright[pos]*smileys[0][i][j]);
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

void wait_a_bit(void);
void wait_a_bit(void) {
	int i;

	for (i = 0; i < 10000000; i++) {	/* Wait a bit. */
		__asm__("nop");

	}
}


int main(void)
{
//volatile uint32_t   dly;
volatile uint32_t  i=0;

  /* we want 48 MHz sysclk */	
  rcc_clock_setup_in_hsi_out_48mhz();
usart_init();
send_char(USART_DIR_UP,'N');
send_char(USART_DIR_RIGHT,'E');
send_char(USART_DIR_DOWN,'S');
send_char(USART_DIR_LEFT,'W');
  sled_init();
	ticker_init();
  disp_init();
  jstick_init();

///	for(dly = 0; dly < 100000; dly++);
//  disp_set(0,7,255);
//  disp_set(0,4,1);
//  disp_set(4,4,1);
//  disp_set(4,0,1);
//  disp_set(1,1,1);

//	disp_set(2,2,1);
//	disp_set(4,4,31);
//

	  for ( i = 0; i < 18; i++) {
		  print_smile(i);
		  sled_on();
		  ticker_msleep(20);
		  sled_off();
		  ticker_msleep(20);
	  }

/*
	  for ( i = 0; i < 4; i++) {
		  print_grey(i);
		  led_on();
		  ticker_msleep(700);
		  led_off();
		  ticker_msleep(700);
	  }

*/
	usart_send_blocking(USART3,'b');
	disp_clean();

		  sled_on();
while (1)
  {
//	dots_worker();
	jstick_worker();
		  ticker_msleep(100);
  }

}
