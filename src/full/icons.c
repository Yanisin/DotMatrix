#include "icons.h"
#include "disp.h"

void draw_icon(icon_data icon, bool invert)
{
	uint8_t xor = invert ? 0xFF : 0x00;
	for(int i = 0; i < 8*8; i++) {
		uint8_t val = icon[i / 8];
		val >>= i % 8;
		val ^= xor;
		disp_set_noupdate(i % 8, i / 8, (val & 1) * 31);
	}
	disp_update_rows();
}

#define _ 0
#define X 1

#if 0
icon_data example = {
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
};
#endif

icon_data icon_arrow = {
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, _, _, X, X, _, _, _),
	ICON_ROW(_, _, _, X, X, _, _, _),
	ICON_ROW(_, _, X, _, _, X, _, _),
	ICON_ROW(_, _, X, _, _, X, _, _),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
};

icon_data icon_square = {
	ICON_ROW(_, _, _, _, _, _, _, _),
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, X, X, X, X, X, X, _),
	ICON_ROW(_, _, _, _, _, _, _, _),
};

icon_data  smiley = {
	ICON_ROW(_, _, X, X, X, X, _, _),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(X, _, X, _, _, X, _, X),
	ICON_ROW(X, _, X, _, _, X, _, X),
	ICON_ROW(X, X, _, _, _, _, X, X),
	ICON_ROW(X, _, X, X, X, X, _, X),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(_, _, X, X, X, X, _, _),
};

icon_data  smiley_sad = {
	ICON_ROW(_, _, X, X, X, X, _, _),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(X, _, X, _, _, X, _, X),
	ICON_ROW(X, _, _, _, _, _, _, X),
	ICON_ROW(X, _, X, X, X, X, _, X),
	ICON_ROW(X, X, _, _, _, _, X, X),
	ICON_ROW(_, X, _, _, _, _, X, _),
	ICON_ROW(_, _, X, X, X, X, _, _),
};
