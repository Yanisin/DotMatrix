#include <stdint.h>
#include <util/icons.h>
#include <applets/applet.h>
#include <hw/disp.h>

/**
 *  Simple Hello World Applet example
 *
 *  Only think you need to do for adding applet
 *  is that you declare the const struct xxx_applet,
 *  and add it to the list of applets via applet_add().
 *  Last think you need to do is to add it to MODS
 *  variable in the Makefile.
 *
 */


/* do all the needed one time init stuff */
static void hello_init(void)
{
	disp_clean();
}

/* called once, once the applet is started */
static void hello_worker(void)
{
	static int i = 0;
	TIME_IIN

	hello_init();

	/* while not asked to exit do our work */
	while (!applet_should_end) {
		/* just light some dot on matrix for a while */
		disp_set(i, 1, 31);
		/* use the ChibiOS sleep */
		chThdSleep(TIME_MS2I(500));
		/* and turn the LED off again */
		disp_set(i, 1, 0);
		i++;
		if (i > 7) {
			i = 0;
		}
	}
}

/* declare the applet */
static const struct applet hello_applet = {
	.run = hello_worker,
	.icon = {
		ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
		ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
		ICON_ROW(1, 0, 1, 0, 1, 0, 0, 1),
		ICON_ROW(1, 0, 1, 0, 0, 0, 0, 1),
		ICON_ROW(1, 1, 1, 0, 1, 0, 0, 1),
		ICON_ROW(1, 0, 1, 0, 1, 0, 0, 0),
		ICON_ROW(1, 0, 1, 0, 1, 0, 0, 1),
		ICON_ROW(0, 0, 0, 0, 0, 0, 0, 0),
	}
};

/* and let it be used it */
applet_add(hello);




