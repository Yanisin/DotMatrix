#ifndef ICONS_H
#define ICONS_H

#include <stdbool.h>
#include <stdint.h>

typedef const uint8_t icon_data[8];

#define ICON_ROW(a, b, c, d, e, f, g, h) \
	(((a) << 0) | ((b) << 1) | ((c) << 2) | ((d) << 3) \
	| ((e) << 4) | ((f) << 5) | ((g) << 6) | ((h) << 7))

enum blit_ops {
	BLIT_SET,
	BLIT_NOT,
	BLIT_OR,
	BLIT_AND,
	BLIT_XOR,
};

void draw_icon(icon_data icon, enum blit_ops ops);

extern icon_data icon_arrow;
extern icon_data icon_edge_line;
extern icon_data icon_top_edges;
extern icon_data icon_square;
extern icon_data icon_master;
extern icon_data smiley;
extern icon_data smiley_sad;

#endif // ICONS_H
