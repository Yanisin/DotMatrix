# DotMatrix

## What is this?

A STM32F070CB driving a LED DotMatrix.

## Prerequisities

  * You need an appropriate Cortex-M ARM toolchain. We suggest to use the
  ARM's [GNU Embedded Toolchain for Arm ](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads).
  Please note that the `gcc-arm-none-eabi` provided by Ubuntu 18.04 is currently broken and can't be used.

  * This project makes use of [libopencm3]()https://github.com/libopencm3/libopencm3) and [ChibiOS](http://www.chibios.org).
  In order to build `libopencm3` you need `Python 2`.

  * To download the firmware, you have two options:
     - Via USB: You need `dfu-util 0.9+` and a USB cable. Please not that `dfu-util 0.8` will not work.
     - Via SWD: You need `openocd` and a `ST-LINK v2 device`
  
  * Optional: `Docker`

You might be interested in some [libopencm3 examples here](https://github.com/libopencm3/libopencm3-examples).
The pull requests there might be an interesting source of information as well.

 
## Install & Build

Get the code:

    $ git clone https://github.com/libopencm3/libopencm3.git
    $ git clone https://github.com/ChibiOS/ChibiOS.git
    $ git clone https://github.com/Yanisin/DotMatrix.git

We provide a Dockerfile, containing the suggested toolchain, openocd, dfu-util and other tools as well.
You can build the container with:

    $ cd DotMatrix/
    DotMatrix$ docker build -t gcc-arm-none-eabi-8-2018-q4 .

Create a file named `settings.mk` in `DotMatrix/src` and provide an appropriate configuration.
You can use `DotMatrix/src/settings.mk.example` as input. This `settings.mk` works with the Docker container:

```
OPENCM3_DIR ?= /home/dev/app/src/libopencm3
CHIBIOS ?= /home/dev/app/src/ChibiOS
HW_VERSION = "HW_60x60_rev_0_5"
SUDO=
SIM=0
OOCD ?= openocd
OOCD_INTERFACE ?= stlink-v2-1
OOCD_TARGET ?= stm32f0x
```

You can start the container with:

    DotMatrix$ docker run -it --privileged --expose 4444 \
                  -v /dev/bus/usb:/dev/bus/usb \
                  -v `pwd`/../libopencm3:/home/dev/app/src/libopencm3 \
                  -v `pwd`/../ChibiOS:/home/dev/app/src/ChibiOS \
                  -v `pwd`:/home/dev/app \
                  gcc-arm-none-eabi-8-2018-q4:latest /bin/bash

You need to compile `libopencm3` before usage. This has to be done once.
With help of the toolchain container this is:

    dev@59e291eed30d:~/app$ cd src/libopencm3/
    dev@59e291eed30d:~/app/src/libopencm3$ make

ChibiOS is compiled on demand.

To compile the firmware / demo of your interest, just go to the directory and issue `$ make`. For instance:

    dev@59e291eed30d:~/app$ cd src/full/
    dev@59e291eed30d:~/app/src/full$ make

This should result in a binary `build/full.elf`.     
    

## Flashing

  * Via SWD: Attach the SWD interface of the MCU via stlink-v2 programmer and issue `$ make prg`
  * Via USB / dfu-utils: Connect the board via USB and issue `$ make prg_usb`
  
See `src/common.mk` for other commands.

# Hardware Errata

## 38x38_monochrome rev. 0.1

- U3 - NMR and NOE are exchanged
- J5 and J4 - USART_TX and RX are exchanged 
- J8 and J9 - USART_TX and RX are exchanged 
- The copy of J3 (USB) on bottom does not have the GND pin connected
- BOOT0 pin floating, shoud be tight to ground

## 60x60_monochrome (rev. 0.2)

- the mounting holes are not in 10x10 matrix
