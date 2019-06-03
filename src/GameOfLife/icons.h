#ifndef ICONS_H
#define ICONS_H

#include <stdbool.h>
#include <stdint.h>

typedef const uint8_t icon_data[8];

#define ICON_ROW(a, b, c, d, e, f, g, h) \
	(((a) << 7) | ((b) << 6) | ((c) << 5) | ((d) << 4) \
	| ((e) << 3) | ((f) << 2) | ((g) << 1) | ((h) << 0))

void draw_icon(icon_data icon, bool invert);

extern icon_data icon_arrow;
extern icon_data icon_square;

#endif // ICONS_H
