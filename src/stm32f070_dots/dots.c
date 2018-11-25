#include <stdint.h>
#include "dots.h"
#include "rand.h"
#include "disp.h"
#include "usart_buffered.h"

#include <libopencm3/stm32/gpio.h>
#define MAX_DOTS 16
#define DOT_ADD_TRESHOLD 1
#define EVERY_N_TICK 150

typedef struct dot_s {
	uint8_t used;
	uint8_t x;
	uint8_t y;
	uint8_t direction;
} dot_t;


static uint8_t dots_num;
static dot_t dots[MAX_DOTS];

void dots_init(void)
{
	int i;
	for (i=0;i<MAX_DOTS;i++) {
		dots[i].used = 0;
	}
}

static void add_dot(void)
{
	int i;
	for (i=0;i<MAX_DOTS;i++) {
		if(dots[i].used == 0) {
			break;
		}
	}
#if 1
	dots[i].x = rand_get()%8;
	dots[i].y = rand_get()%8;
	dots[i].direction = rand_get()%4;
#else
	dots[i].x = 4;
	dots[i].y = 4;
	dots[i].direction = 2;
#endif
	dots[i].used = 1;
	dots_num++;
}

static void receive_new(void)
{
 uint32_t c;
	int i;

	while ((c=get_char(USART_DIR_UP)) != (uint32_t)-1) {
		if(c < '0' || c >'7' ) {
			continue;
		}
//		send_char(USART_DIR_UP,'(');
//		send_char(USART_DIR_UP,c);
//		send_char(USART_DIR_UP,')');
		for (i=0;i<MAX_DOTS;i++) {
			if(dots[i].used == 0) {
				dots[i].x = c-'0';
				dots[i].y = 0;
				dots[i].direction = 3;
				dots[i].used = 1;
	dots_num++;
				break;
			}
		}
	}

	while ((c=get_char(USART_DIR_DOWN)) != (uint32_t)-1) {
		if(c < '0' || c >'7' ) {
			continue;
		}
		for (i=0;i<MAX_DOTS;i++) {
			if(dots[i].used == 0) {
				dots[i].x = c-'0';
				dots[i].y = 7;
				dots[i].direction = 2;
				dots[i].used = 1;
	dots_num++;
				break;
			}
		}
	}

	while ((c=get_char(USART_DIR_RIGHT)) != (uint32_t)-1) {
		if(c < '0' || c >'7' ) {
			continue;
		}
		for (i=0;i<MAX_DOTS;i++) {
			if(dots[i].used == 0) {
				dots[i].x = 7;
				dots[i].y = c-'0';
				dots[i].direction = 0;
				dots[i].used = 1;
				dots_num++;
				break;
			}
		}
	}

	while ((c=get_char(USART_DIR_LEFT)) != (uint32_t)-1) {
//		send_char(USART_DIR_LEFT,'(');
//		send_char(USART_DIR_LEFT,c);
//		send_char(USART_DIR_LEFT,')');
		if(c < '0' || c >'7' ) {
			continue;
		}
		for (i=0;i<MAX_DOTS;i++) {
			if(dots[i].used == 0) {
				dots[i].x = 0;
				dots[i].y = c-'0';
				dots[i].direction = 1;
				dots[i].used = 1;
				dots_num++;
				break;
			}
		}
	}
}


static void hide_dots(void)
{
	int i;
	/* switch all off */
	for (i=0;i<MAX_DOTS;i++) {
		if(dots[i].used != 0) {
			disp_set(dots[i].x,dots[i].y,0);
		}
	}


}
static void show_dots(void)
{
	int i;
		/* draw */
	for (i=0;i<MAX_DOTS;i++) {
		if(dots[i].used != 0) {
			disp_set(dots[i].x,dots[i].y,255);
		}
	}


}

static void move_dots(void)
{
	int i;
	/* recalculate */

	for (i=0;i<MAX_DOTS;i++) {
		if(dots[i].used != 0) {
			switch(dots[i].direction) {
				case 0:
					if(dots[i].x == 0) {
						send_char(USART_DIR_LEFT,dots[i].y+'0');
						dots[i].used = 0;
						dots_num--;
					} else {
						dots[i].x--;
					}
					break;
				case 1:
					if(dots[i].x == 7) {
						send_char(USART_DIR_RIGHT,dots[i].y+'0');
						dots[i].used = 0;
						dots_num--;
					} else {
						dots[i].x++;
					}
					break;
				case 2:
					if(dots[i].y == 0) {
						dots[i].used = 0;
						send_char(USART_DIR_UP,dots[i].x+'0');
						dots_num--;
					} else {
						dots[i].y--;
					}
					break;
				case 3:
					if(dots[i].y == 7) {
						send_char(USART_DIR_DOWN,dots[i].x+'0');
						dots[i].used = 0;
						dots_num--;
					} else {
						dots[i].y++;
					}
					break;
			}
		}
	}

}

void dots_tick(void)
{
	static int i=0;
	i++;
	if (i >= EVERY_N_TICK)
	{
	gpio_toggle(GPIOF, GPIO0);
		i=0;
		hide_dots();
		move_dots();
		receive_new();
		show_dots();
		if(dots_num < DOT_ADD_TRESHOLD) {
			add_dot();
		}
	}
}

void dots_worker(void)
{

}
