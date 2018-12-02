/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 * Modified by Fernando Cortes <fermando.corcam@gmail.com>
 * modified by Guillermo Rivera <memogrg@gmail.com>
 * modified by Frantisek Burian <BuFran@seznam.cz>
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
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>

#include "hw_defs.h"
#include "disp.h"
#include "jstick.h"

#define JSTICK_CAL_X_MIN 0
#define JSTICK_CAL_X_MAX 4000
#define JSTICK_CAL_Y_MIN 0
#define JSTICK_CAL_Y_MAX 4000

uint8_t channel_array[] = { 2, 3 };

static void jstick_adc_init(void)
{
	rcc_periph_clock_enable(RCC_ADC);
	rcc_periph_clock_enable(RCC_GPIOA);

	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO2);
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO3);

	adc_power_off(ADC1);
	adc_set_clk_source(ADC1, ADC_CLKSOURCE_ADC);
	adc_calibrate(ADC1);
	adc_set_operation_mode(ADC1, ADC_MODE_SCAN);
	adc_disable_external_trigger_regular(ADC1);
	adc_set_right_aligned(ADC1);
	adc_enable_temperature_sensor();
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPTIME_071DOT5);
	adc_set_regular_sequence(ADC1, 2, channel_array);
	adc_set_resolution(ADC1, ADC_RESOLUTION_12BIT);
	adc_disable_analog_watchdog(ADC1);
	adc_power_on(ADC1);

	/* Wait for ADC starting up. */
	int i;
	for (i = 0; i < 800000; i++) {    /* Wait a bit. */
		__asm__("nop");
	}

}


static void jstick_btn_init(void)
{
	/* Enable GPIOA clock. */
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOA);


	/* Set GPIO0 (in GPIO port A) to 'input open-drain'. */
	gpio_mode_setup(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO14);
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO4);

}

uint32_t jstick_get_x(void)
{
	uint32_t temp;
		adc_start_conversion_regular(ADC1);
		while (!(adc_eoc(ADC1)));

		temp = adc_read_regular(ADC1);
		 (void) adc_read_regular(ADC1);

	return temp;
}
uint32_t jstick_get_y(void)
{
	uint32_t temp;
		adc_start_conversion_regular(ADC1);
		while (!(adc_eoc(ADC1)));

		temp = adc_read_regular(ADC1);
		temp = adc_read_regular(ADC1);

	return temp;
}

void jstick_init(void )
{
jstick_btn_init();
jstick_adc_init();
}

void jstick_worker(void)
{
	uint32_t x;
	uint32_t y;
	static uint32_t dot_x = 0;
	static uint32_t dot_y = 0;

		if (gpio_get(GPIOA, GPIO4)) {
			disp_set(0,7,0);
			} else {
			disp_set(0,7,255);
			}
		if (gpio_get(GPIOC, GPIO14)) {
			disp_set(7,7,0);
			} else {
			disp_set(7,7,255);
			}
		x = jstick_get_x();
		y = jstick_get_y();

	disp_set(dot_x, dot_y,0);	

	dot_x = DISP_COLS_NUM * (x - JSTICK_CAL_X_MIN)/ (JSTICK_CAL_X_MAX - JSTICK_CAL_X_MIN);

	dot_y = DISP_ROWS_NUM * (y - JSTICK_CAL_Y_MIN)/ (JSTICK_CAL_Y_MAX - JSTICK_CAL_Y_MIN);
	if (dot_x > 7) {
		dot_x = 7;
	}
	if (dot_y > 7) {
		dot_y = 7;
	}
	dot_x = DISP_COLS_NUM- 1 - dot_x;
	dot_y = DISP_ROWS_NUM- 1 - dot_y;
 	
	disp_set(dot_x, dot_y,255);	

}
