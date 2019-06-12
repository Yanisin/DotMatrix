#ifndef MGMT_MSG_H
#define MGMT_MSG_H

#include <stdint.h>
#include "cell_id.h"
#include "usart_buffered.h"

#define MGMT_BL_START MSG_MGMT_ID(0)
struct mgmt_bl_start {
};

/*
 * Request the management layer to switch to the choice applet
 */
#define MGMT_INTERRTUP MSG_MGMT_ID(1)
struct mgmt_interrupt {};
/*
 * Request all cells to change the current applet
 */
#define MGMT_CHANGE_APPLET MSG_MGMT_ID(2)
struct mgmt_change_applet {
       uint8_t applet;
};

#define MGMT_NOP MSG_MGMT_ID(3)


#endif // MGMT_MSG_H
