
# Directions (RX and TX) are decsribed from the point of the device

MSGID_CLIENT_HELLO = 1
MSGID_UART_TX = 2
MSGID_UART_RX = 3
MSGID_LED_STATE = 4
MSGID_UPDATE_DISP = 5
MSGID_GPIO_STATE = 6
MSGID_GPIO_INPUT_STATE = 7
MSGID_BUTTON_DOWN = 8
MSGID_BUTTON_UP = 9

DIR_RIGHT=0
DIR_DOWN=1
DIR_LEFT=2
DIR_UP=3

def list_defs():
    print('#pragma once')
    for k, v in globals().items():
        if k.startswith('MSGID_'):
            print('#define {} {}'.format(k, v))

if __name__ == "__main__":
    list_defs()