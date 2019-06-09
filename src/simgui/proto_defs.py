
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
MSGID_I2C_TX = 10
MSGID_I2C_RX = 11

DIR_UP=0
DIR_RIGHT=1
DIR_DOWN=2
DIR_LEFT=3

def list_defs():
    print('#pragma once')
    for k, v in globals().items():
        if k.startswith('MSGID_'):
            print('#define {} {}'.format(k, v))

if __name__ == "__main__":
    list_defs()