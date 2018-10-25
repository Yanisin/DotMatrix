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

