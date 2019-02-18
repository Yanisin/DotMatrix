#include <stdint.h>
#include <string.h>

#include <libopencm3/stm32/rtc.h>

#include "ticker.h"
#include "disp.h"
#include "wordclock.h"
#include "console.h"

static unsigned int clock_tick_next;

enum words {
	TWELVE,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	ELEVEN,
	MTWENTY,
	MQUARTER,
	MFIVE,
	MTEN,
	MHALF,
	MPAST,
	MTO
};

static const uint8_t words[][8] = {
		[ONE] = 	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49 },
		[TWO] = 	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x40 },
		[THREE] = 	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00 },
		[FOUR] = 	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0 },
		[FIVE] = 	{ 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00 },
		[SIX] = 	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00 },
		[SEVEN] = 	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0F, 0x00 },
		[EIGHT] =	{ 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00 },
		[NINE] = 	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F },
		[TEN] = 	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x89, 0x00 },
		[ELEVEN] =	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x00 },
		[TWELVE] =	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF6, 0x00 },
		[MTWENTY] = { 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		[MQUARTER]=	{ 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		[MFIVE] = 	{ 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00 },
		[MTEN] = 	{ 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		[MHALF] = 	{ 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00 },
		[MPAST] = 	{ 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00 },
		[MTO] =		{ 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00 }
};

static enum words word_current;

static void draw(const uint8_t *ca, uint8_t val)
{
	for (int row = 0; row < 8; ++row) {
		for (int col = 0; col < 8; ++col) {
			int br = !!(ca[row] & (1 << (7-col))) * val;
			disp_set_noupdate(col, row, br);
		}
		disp_update_rows();
	}
}

static void display_word(int ci, uint8_t val)
{
	if (ci < ONE || ci > MTO)
		return;

	draw(words[ci], val);
}

static void update_mask(uint8_t mask[8], enum words word)
{
	for (int i = 0; i < 8; ++i)
		mask[i] |= words[word][i];
}

static int bcd_to_int(int bcd)
{
	return (bcd & 0x0F) + ((bcd >> 4) & 0x0F) * 10;
}

void clock_init(void)
{
	clock_tick_next = ticker_get_ticks() + 1000;
	word_current = ONE;
}

void clock_worker(void)
{
	unsigned int tick = ticker_get_ticks();
	uint8_t mask[8];
	int rtc_time;
	int hrs;
	int mins;

	if ((int)(tick - clock_tick_next) < 0)
		return;

#ifdef TEST
	// test function, cycle through all words
	display_word(word_current, 31);
	++word_current;
	if (word_current > MTO)
		word_current = ONE;
#else
	memset(mask, 0, sizeof(mask));
	rtc_time = RTC_TR;

	hrs = bcd_to_int((rtc_time >> RTC_TR_HU_SHIFT) & 0xFF);
	mins = bcd_to_int((rtc_time >> RTC_TR_MNU_SHIFT) & 0xFF);

	console_printf("%02i:%02i\n", hrs, mins);

	if (mins >= 5 && mins < 10) {
		update_mask(mask, MFIVE);
	}
	if (mins >= 10 && mins < 15) {
		update_mask(mask, MTEN);
	}
	if (mins >= 15 && mins < 20) {
		update_mask(mask, MQUARTER);
	}
	if (mins >= 20 && mins < 25) {
		update_mask(mask, MTWENTY);
	}
	if (mins >= 25 && mins < 30) {
		update_mask(mask, MTWENTY);
		update_mask(mask, MFIVE);
	}
	if (mins >= 30 && mins < 35) {
		update_mask(mask, MHALF);
	}
	if (mins >=35 && mins < 40) {
		update_mask(mask, MTWENTY);
		update_mask(mask, MFIVE);
	}
	if (mins >= 40 && mins < 45) {
		update_mask(mask, MTWENTY);
	}
	if (mins >= 45 && mins < 50) {
		update_mask(mask, MQUARTER);
	}
	if (mins >= 50 && mins < 55) {
		update_mask(mask, MTEN);
	}
	if (mins >= 55 && mins != 0) {
		update_mask(mask, MFIVE);
	}

	if (mins >= 35 && mins != 0) {
		++hrs;
		update_mask(mask, MTO);
	} else
	if (mins >= 5) {
		update_mask(mask, MPAST);
	}

	update_mask(mask, hrs % 12);

	draw(mask, 31);
#endif

	clock_tick_next = tick + 1000;
}
