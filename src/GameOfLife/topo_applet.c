#include "topo.h"
#include "applet.h"
#include "disp.h"
#include "icons.h"

static void topo_init(void)
{
	disp_set_rotation(topo_master_direction);
	disp_clean();
	if (topo_is_master) {
		draw_icon(icon_square, false);
	} else {
		draw_icon(icon_arrow, false);
	}
}

static void topo_worker(void)
{
}

const struct applet topo_applet = {
	.init = topo_init,
	.worker = topo_worker,
};
