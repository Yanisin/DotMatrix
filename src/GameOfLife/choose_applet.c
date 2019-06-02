#include "usart_buffered.h"

#define MSG_SHOW_APPLET MSG_USER_ID(0)
struct msg_show_applet {
	uint8_t cell_id;
	uint8_t applet;
};
