#pragma once

/**
 * @file
 *  Provides standard icons, icon drawing and icon creation helpers
 */

#include <stdbool.h>
#include <stdint.h>

/**
 * Icons are stored in row-major format, with each row stored as one byte. Each bit is one pixel.
 */
typedef const uint8_t icon_data[8];

/**
 * Use to define each ROW of icon.
 */
#define ICON_ROW(a, b, c, d, e, f, g, h) \
	(((a) << 0) | ((b) << 1) | ((c) << 2) | ((d) << 3) \
	| ((e) << 4) | ((f) << 5) | ((g) << 6) | ((h) << 7))

/**
 * Defines how to combine icon with the old screen content.
 */
enum blit_ops {
	/** Clean old screen content, just display the icon */
	BLIT_SET,
	/** Clean old screen content, display the icon inverted */
	BLIT_NOT,
	/** Logical OR of screen content and icon */
	BLIT_OR,
	/** Logical AND of screen content and icon */
	BLIT_AND,
	/** Logical XOR of screen content and icon */
	BLIT_XOR,
};

/**
 * Draw icon to the display. Automatically updates the display.
 */
void draw_icon(icon_data icon, enum blit_ops ops);

extern icon_data icon_arrow;
extern icon_data icon_edge_line;
extern icon_data icon_top_edges;
extern icon_data icon_square;
extern icon_data icon_master;
extern icon_data smiley;
extern icon_data smiley_sad;
