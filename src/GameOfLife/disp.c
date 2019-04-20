#include "stdint.h"

#ifdef STDPERIPH
#include "stm32f0xx.h"
#else

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#endif


#include "disp.h"
#include "disp_spi.h"
#include "hw_defs.h"
#define COLOR_DEPTH 32

static uint8_t disp_state[DISP_ROWS_NUM][DISP_COLS_NUM];
static uint8_t curr_row;

static void disp_tick(void);

#ifdef STDPERIPH


#else
void tim17_isr(void)
{
//	static uint32_t baah=0;
	if (timer_get_flag(TIM17,TIM_SR_UIF)) {
		timer_clear_flag(TIM17, TIM_SR_UIF);
		//		if(++baah == 1) {
				 disp_tick();
		//		 baah=0;
		//		}

	}
}


static void tim17_init(void);
static void tim17_init(void)
{
    /* Enable TIM2 clock. */
    rcc_periph_clock_enable(RCC_TIM17);

    /* Enable TIM2 interrupt. */
    nvic_enable_irq(NVIC_TIM17_IRQ);

    /* Reset TIM2 peripheral to defaults. */
    rcc_periph_reset_pulse(RST_TIM17);

    /* Timer global mode:
     * - No divider
     * - Alignment edge
     * - Direction up
     * (These are actually default values after reset above, so this call
     * is strictly unnecessary, but demos the api for alternative settings)
     */
    timer_set_mode(TIM17, TIM_CR1_CKD_CK_INT,
        TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

    /*
     * Please take note that the clock source for STM32 timers
     * might not be the raw APB1/APB2 clocks.  In various conditions they
     * are doubled.  See the Reference Manual for full details!
     * In our case, TIM2 on APB1 is running at double frequency, so this
     * sets the prescaler to have the timer run at 5kHz
     */
    //timer_set_prescaler(TIM17, ((rcc_apb1_frequency * 2) / 5000));
    //timer_set_prescaler(TIM17, 1);
    timer_set_prescaler(TIM17, 1);

    /* Disable preload. */
    timer_disable_preload(TIM17);
//    timer_enable_preload(TIM17);
    timer_continuous_mode(TIM17);

    /* count full range, as we'll update compare value continuously */
    timer_set_period(TIM17, 1000);
//    timer_set_period(TIM17, 250);


    /* Counter enable. */
    timer_enable_counter(TIM17);

	timer_enable_irq(TIM17, TIM_DIER_UIE);


}

static void disp_out_init(void);
static void disp_out_init(void) 
{

	rcc_periph_clock_enable(RCC_GPIOA |RCC_GPIOB);
	gpio_mode_setup(DISP_GPIO_ROW0, 
		GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_GPIOP_ROW0);
	gpio_mode_setup(DISP_GPIO_ROW1, 
		GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_GPIOP_ROW1);
	gpio_mode_setup(DISP_GPIO_ROW2, 
		GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_GPIOP_ROW2);
	gpio_mode_setup(DISP_GPIO_ROW3, 
		GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_GPIOP_ROW3);
	gpio_mode_setup(DISP_GPIO_ROW4, 
		GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_GPIOP_ROW4);
	gpio_mode_setup(DISP_GPIO_ROW5, 
		GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_GPIOP_ROW5);
	gpio_mode_setup(DISP_GPIO_ROW6, 
		GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_GPIOP_ROW6);
	gpio_mode_setup(DISP_GPIO_ROW7, 
		GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DISP_GPIOP_ROW7);
}

void row_off(uint8_t pos);
void row_on(uint8_t pos);
void row_on(uint8_t pos)
{
	switch (pos) {
		case 0:
			gpio_set(DISP_GPIO_ROW0, DISP_GPIOP_ROW0);
		break;
		case 1:
			gpio_set(DISP_GPIO_ROW1, DISP_GPIOP_ROW1);
		break;
		case 2:
			gpio_set(DISP_GPIO_ROW2, DISP_GPIOP_ROW2);
		break;
		case 3:
			gpio_set(DISP_GPIO_ROW3, DISP_GPIOP_ROW3);
		break;
		case 4:
			gpio_set(DISP_GPIO_ROW4, DISP_GPIOP_ROW4);
		break;
		case 5:
			gpio_set(DISP_GPIO_ROW5, DISP_GPIOP_ROW5);
		break;
		case 6:
			gpio_set(DISP_GPIO_ROW6, DISP_GPIOP_ROW6);
		break;
		case 7:
			gpio_set(DISP_GPIO_ROW7, DISP_GPIOP_ROW7);
		break;
	}
}

void row_off(uint8_t pos)
{
	switch (pos) {
		case 0:
			gpio_clear(DISP_GPIO_ROW0, DISP_GPIOP_ROW0);
		break;
		case 1:
			gpio_clear(DISP_GPIO_ROW1, DISP_GPIOP_ROW1);
		break;
		case 2:
			gpio_clear(DISP_GPIO_ROW2, DISP_GPIOP_ROW2);
		break;
		case 3:
			gpio_clear(DISP_GPIO_ROW3, DISP_GPIOP_ROW3);
		break;
		case 4:
			gpio_clear(DISP_GPIO_ROW4, DISP_GPIOP_ROW4);
		break;
		case 5:
			gpio_clear(DISP_GPIO_ROW5, DISP_GPIOP_ROW5);
		break;
		case 6:
			gpio_clear(DISP_GPIO_ROW6, DISP_GPIOP_ROW6);
		break;
		case 7:
			gpio_clear(DISP_GPIO_ROW7, DISP_GPIOP_ROW7);
		break;
	}
}


void sink_buff_enable(void);
void sink_buff_enable(void)
{
}

void sink_buff_disable(void);
void sink_buff_disable(void)
{
}

#endif




void disp_init(void)
{
	disp_clean();
	disp_out_init();
	//sink_buff_enable();
	curr_row=0;
	tim17_init();
	disp_spi_init();
}

static void disp_tick(void) {
	static int progress=0;
	uint8_t i;
	uint8_t col;
//		gpio_toggle(GPIOF, GPIO0);	/* LED on/off */
	/* turn off current row */
	if(progress == 0) {
		row_off(curr_row);
		/* set new row */
		curr_row++;
		if (curr_row >= DISP_ROWS_NUM) {
			curr_row = 0;
		}
	}
	if (progress > 1 ) {
			col=0;
		for(i=0;i<DISP_COLS_NUM;i++) {
		if(disp_state[curr_row][i] > progress-1) {
//			if(disp_state[curr_row][i]) {
//				col_on(i);
	#ifdef DISP_COLS_DATA_BACKWARDS
				col |= 1<<((DISP_COLS_NUM-1)-i);

	#else
				col |= 1<<i;
	#endif

//			}else {
				//col_off(i);
			}
//		}
		}
//		if(curr_row == 7) {
//		disp_spi_col_set(0xff);
//		} else {
//			disp_spi_col_set(col | 0x1);
			disp_spi_col_set(col);
//		}
		//disp_spi_col_set(0xC0+curr_row);
	row_on(curr_row);
	}

	/* turn on new row */
	progress++;
	if (progress==COLOR_DEPTH) {
	progress=0;
	}
}



void disp_clean(void)
{
	int i,j;
	for (i=0;i<DISP_ROWS_NUM;i++) {
		for (j=0;j<DISP_COLS_NUM;j++) {
			disp_state[i][j]=0;
		}
	}
}
void disp_set(uint8_t col, uint8_t row, uint8_t val)
{
	disp_state[row][col]=val;
}
