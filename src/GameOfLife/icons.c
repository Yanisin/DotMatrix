#include "icons.h"
#include "disp.h"

void draw_icon(icon_data icon, bool invert)
{
	uint8_t xor = invert ? 0xFF : 0x00;
	for(int i = 0; i < 8*8; i++) {
		uint8_t val = icon[i / 8];
		val >>= i % 8;
		val ^= xor;
		disp_set(i % 8, i / 8, (val & 1) * 31);
	}
}


#if 0
icon_data example = {
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
};
#endif

icon_data icon_arrow = {
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 0, 0, 1, 1, 0, 0, 0),
	ICON_ROW(0, 0, 0, 1, 1, 0, 0, 0),
	ICON_ROW(0, 0, 1, 0, 0, 1, 0, 0),
	ICON_ROW(0, 0, 1, 0, 0, 1, 0, 0),
	ICON_ROW(0, 1, 0, 0, 0, 0, 1, 0),
	ICON_ROW(0, 1, 0, 0, 0, 0, 1, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
};

icon_data icon_square = {
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	ICON_ROW(0, 1, 1, 1, 1, 1, 1, 0),
	ICON_ROW(0, 1, 1, 1, 1, 1, 1, 0),
	ICON_ROW(0, 1, 1, 1, 1, 1, 1, 0),
	ICON_ROW(0, 1, 1, 1, 1, 1, 1, 0),
	ICON_ROW(0, 1, 1, 1, 1, 1, 1, 0),
	ICON_ROW(0, 1, 1, 1, 1, 1, 1, 0),
	ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
};
