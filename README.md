# DotMatrix

## Prerequisities
* you need to install openocd and gcc-arm-none-eabi and maybe something more

* you will need to download and compile https://github.com/libopencm3/libopencm3

you might be interested in some libopencm3 examples here:
https://github.com/libopencm3/libopencm3-examples
the pull requests there might be an interesting source of informations as well


## Build

in rules.mk set path to the libopencm3 directory in Makefiles.

attach the SWD interface of the MCU via stlink-v2 programmer and issue "make prg" 

# Errata

## 38x38_monochrome rev. 0.1

- U3 - NMR and NOE are exchanged
- J5 and J4 - USART_TX and RX are exchanged 
- J8 and J9 - USART_TX and RX are exchanged 
- The copy of J3 (USB) on bottom does not have the GND pin connected
- BOOT0 pin floating, shoud be tight to ground
