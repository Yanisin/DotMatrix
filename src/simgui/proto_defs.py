
MSGID_CLIENT_HELLO = 1
MSGID_UART_TX = 2
MSGID_UART_RX = 3
MSGID_LED_STATE = 4
MSGID_UPDATE_DISP = 5

def list_defs():
    print('#pragma once')
    for k, v in globals().items():
        if k.startswith('MSGID_'):
            print('#define {} {}'.format(k, v))

if __name__ == "__main__":
    list_defs()