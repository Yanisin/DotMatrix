#include <applets/applet.h>


static const struct applet cell_1d_applet = {
	.run = cell_1d_run,
	.autostart = true,
};

applet_add(cell_1d);
