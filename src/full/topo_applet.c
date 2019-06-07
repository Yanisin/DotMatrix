#include "topo.h"
#include "applet.h"
#include "disp.h"
#include "icons.h"

static void topo_applet_run(void)
{
	disp_set_rotation(topo_master_direction);
	disp_clean();
	if (topo_is_master) {
		draw_icon(icon_square, false);
	} else {
		draw_icon(icon_arrow, false);
	}

	applet_wait_for_end(TIME_INFINITE);
}

const struct applet topo_applet = {
	.run = topo_applet_run,
	.icon = {
		ICON_ROW(0, 1, 1, 0, 0, 0, 0, 0),
		ICON_ROW(0, 1, 1, 0, 0, 0, 0, 0),
		ICON_ROW(0, 0, 1, 0, 1, 1, 0, 0),
		ICON_ROW(0, 0, 1, 1, 1, 1, 0, 0),
		ICON_ROW(0, 0, 0, 1, 0, 1, 1, 1),
		ICON_ROW(0, 1, 1, 1, 0, 0, 1, 1),
		ICON_ROW(0, 1, 1, 0, 0, 0, 0, 0),
		ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	}
};


applet_add(topo);
