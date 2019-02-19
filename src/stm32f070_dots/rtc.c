#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/cm3/nvic.h>

#include "rtc.h"
#include "console.h"
#include "ticker.h"
#include "cdcacm.h"
#include "applet.h"

enum worker_state {
	WORKER_IDLE,
	WAIT_DIGIT0,
	WAIT_DIGIT1,
	WAIT_DIGIT2,
	WAIT_DIGIT3,
	SET_TIME,
};

static enum worker_state worker_state;
static unsigned int bcd_time;

static void rtc_set_time(unsigned int time)
{
	uint32_t rtc_time;

	/* unlock access to RTC registers */
	pwr_disable_backup_domain_write_protect();
	rtc_unlock();

	/* Enter init mode */
	RTC_ISR |= RTC_ISR_INIT;
	while ((RTC_ISR & RTC_ISR_INITF) == 0)
		;

	/* update the time */
	rtc_time = RTC_TR;
	rtc_time &= 0xFFFF0000;
	rtc_time |= time;
	RTC_TR = rtc_time;

	/* exit init mode */
	RTC_ISR &= ~RTC_ISR_INIT;

	/* lock register access */
	rtc_lock();
	pwr_enable_backup_domain_write_protect();
}

static void rtc_init(void)
{
	rcc_periph_clock_enable(RCC_PWR);
	pwr_disable_backup_domain_write_protect ();
	rcc_osc_on(RCC_LSE);
	rcc_wait_for_osc_ready(RCC_LSE);
	rcc_set_rtc_clock_source(RCC_LSE);
	rcc_enable_rtc_clock();
	pwr_enable_backup_domain_write_protect ();


}

static void rtc_worker(void)
{
	uint32_t c = console_getc();

	if (c == (uint32_t)-1)
		return;

	console_putc(c);

	switch (worker_state) {
	case WORKER_IDLE:
		if (c == 't') {
			worker_state = WAIT_DIGIT0;
			bcd_time = 0;
		}
		break;

	case WAIT_DIGIT0:
	case WAIT_DIGIT1:
	case WAIT_DIGIT2:
	case WAIT_DIGIT3:
		if (c >= '0' && c <= '9') {
			bcd_time |= c - '0';
			bcd_time <<= 4;
			++worker_state;
		} else
			worker_state = WORKER_IDLE;
		break;

	case SET_TIME:
		rtc_set_time(bcd_time << 4);
		worker_state = WORKER_IDLE;
		console_puts("\nSET\n");
		break;

	default:
		break;
	}
}

static const struct applet rtc_applet = {
	.init = rtc_init,
	.worker = rtc_worker,
};

applet_add(rtc);
