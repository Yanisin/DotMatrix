/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 * Copyright (C) 2012 Karl Palsson <karlp@tweak.net.au>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "hw_defs.h"

static void gpio_setup(void)
{
	/* Enable GPIOC clock. */
	/* Manually: */
	//RCC_AHBENR |= RCC_AHBENR_GPIOCEN;
	/* Using API functions: */
	rcc_periph_clock_enable(LED_RCC);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);


	/* Set GPIO8 (in GPIO port C) to 'output push-pull'. */
	/* Using API functions: */
	gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
	gpio_mode_setup(DISP_nOE_ROW_GPIO  , GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,DISP_nOE_ROW_GPIO_PIN);
	gpio_mode_setup(DISP_nOE_COL_GPIO  , GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,DISP_nOE_COL_GPIO_PIN  );
	gpio_mode_setup(DISP_COLS_CLK_GPIO  , GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_COLS_CLK_GPIO_PIN  );
	gpio_mode_setup(DISP_COLS_DATA_GPIO  , GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_COLS_DATA_GPIO_PIN  );
	gpio_mode_setup(DISP_COLS_STROBE_GPIO  , GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_COLS_STROBE_GPIO_PIN  );
}

#define wait \
		for (i = 0; i < 500; i++) {	/* Wait a bit. */\
			__asm__("nop");\
		}\

bitbang(uint8_t col, uint8_t row)
{
	int i;
	int k;
		gpio_clear(LED_PORT, LED_PIN);	/* LED on/off */

		for (k=0;k<8;k++) {
			if(col & (1<<k)) {
				gpio_set(DISP_COLS_DATA_GPIO, DISP_COLS_DATA_GPIO_PIN);
			} else {
				gpio_clear(DISP_COLS_DATA_GPIO, DISP_COLS_DATA_GPIO_PIN);
			}
			__asm__("nop");
			gpio_set(DISP_COLS_CLK_GPIO, DISP_COLS_CLK_GPIO_PIN);
			__asm__("nop");
			gpio_clear(DISP_COLS_CLK_GPIO, DISP_COLS_CLK_GPIO_PIN);
			__asm__("nop");
			}

		for (k=0;k<8;k++) {
			if(row & (1<<k)) {
				gpio_set(DISP_COLS_DATA_GPIO, DISP_COLS_DATA_GPIO_PIN);
			} else {
				gpio_clear(DISP_COLS_DATA_GPIO, DISP_COLS_DATA_GPIO_PIN);
			}
			__asm__("nop");
			gpio_set(DISP_COLS_CLK_GPIO, DISP_COLS_CLK_GPIO_PIN);
			__asm__("nop");
			gpio_clear(DISP_COLS_CLK_GPIO, DISP_COLS_CLK_GPIO_PIN);
			__asm__("nop");
			}

			/*strobe*/
		gpio_set(DISP_COLS_STROBE_GPIO, DISP_COLS_STROBE_GPIO_PIN);
		__asm__("nop");
		gpio_clear(DISP_COLS_STROBE_GPIO, DISP_COLS_STROBE_GPIO_PIN);
		__asm__("nop");
	
		gpio_set(LED_PORT, LED_PIN);	/* LED on/off */
		__asm__("nop");


}


int main(void)
{
	int i;
	int k;

	gpio_setup();
	k=1;

		gpio_clear(DISP_nOE_ROW_GPIO,DISP_nOE_ROW_GPIO);
		gpio_clear(DISP_nOE_COL_GPIO,DISP_nOE_COL_GPIO);
	/* Blink the LED (PC8) on the board. */
	while (1) {
		/* Manually: */
		// GPIOC_BSRR = LED_PIN;		/* LED off */
		// for (i = 0; i < 1000000; i++)	/* Wait a bit. */
		//	__asm__("nop");
		// GPIOC_BRR = LED_PIN;			/* LED on */
		// for (i = 0; i < 1000000; i++)	/* Wait a bit. */
		//	__asm__("nop");

		/* Using API functions gpio_set()/gpio_clear(): */
		// gpio_set(LED_PORT, LED_PIN);		/* LED off */
		// for (i = 0; i < 1000000; i++)	/* Wait a bit. */
		//	__asm__("nop");
		// gpio_clear(LED_PORT, LED_PIN);	/* LED on */
		// for (i = 0; i < 1000000; i++)	/* Wait a bit. */
		//	__asm__("nop");

		/* Using API function gpio_toggle(): */
		bitbang(k,0xff);
		if(k==0x80){
			k=1;
			}
		else {
			k=k<<1;
		}
		 for (i = 0; i < 1000; i++)	/* Wait a bit. */
			__asm__("nop");

	}

	return 0;
}
