# DotMatrix

## What is this?

A STM32F070CB driving a LED DotMatrix.

## Prerequisities for building Firmware

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

You will also want to have the manuals for your hardware at hand. These are the most important for software development:
 - Peripherals, memory layout, registers ... : [STM32F030x4/x6/x8/xC and STM32F070x6/xB MCUs](https://www.st.com/resource/en/reference_manual/dm00091010.pdf)
 - Pinouts, Electric Characteristcs, HW variants, clocks ...:
   [STM32F070CB Product Specification](https://www.st.com/resource/en/datasheet/stm32f070cb.pdf)
 - Exceptions (NVIC), instructions ... :
   [ARM® v7-M Architecture Reference Manual](https://static.docs.arm.com/ddi0403/eb/DDI0403E_B_armv7m_arm.pdf)

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

### Content of the package

The software demos live in the src directory. There are are three main demos
avaiable:

- `src/stm32f070_miniblink` - a simple demo just blinking the LED
- `src/GameOfLife` - GameOfLife with all connected cells working together.
  Simple communication protocol over UART. The demo uses just libopencm3
  for HW interfacing.
- `src/full` - the 'catch all' demo. Includes GameOfLife and other applications
  that are selectable with an 'applet selector'. Provides a communication
  layer over I2C and UART. Automatically detects the topology of the boards
  at startup and chooses a 'master board'. Is based on an RTOS (ChibiOS).

There are also other small testing projects and a simulator that allows you to
run the 'full' demo on your host machine.

## Flashing

Flashing is done using the `make prg` and `make prg_usb` commands.
See `src/common.mk` for other makefile commands.

### SWD Interface

Attach the SWD interface of the MCU via stlink-v2 programmer and issue `$ make prg`

## USB DFU

Via USB / dfu-utils: Issue `$ make prg_usb` and connect the board over USB
(or restart it, if it is already connected. The flashing process will start as
soon as the board reaches the bootlader (you will see threee dots). If multiple
boards are connected and started at once, the firmware will be loaded to all of
them.

The flashing is managed by our own bootloader. You can checkout the code in
`src/dfu`, if you want to. You can program the bootloader using make prg.

## Fallback USB DFU

If you hold the `reset` button for a few seconds, you will get the board into
manufacturer provided DFU mode. Use `make prg_bl` to flash it then. This can be
used to flash the bootloader or have the wonderfull feeling of running
bare-metal. If you want to run without the bootloader, you have to compile
the demo with `NO_BOOTLOADER=1`. Use this mode if there is no STLink.

## Memory layout

TBD

# Hardware and mechanical parts
The currently used/latest HW is described in "pcb/60x60_dma". The board
is designed with KiCAD but there is the schematics exported in PDF 
and some assembly hints with list of needed parts are added as well.
The other folders in "src" are mostly unfinished various experiments/extension.

In the "mechanical" folder you can find the mechanical parts for 3D printing.
For each board PCB you will need one disp_support.stl.  The file is expected to
be printed with 0.4mm nozzle and 0.15mm layer height.  For connecting boards
you can use joint_cross_1_5m.stl for each corner (in older revision of the
board there is LED in the way so you need the use the one with the cuts for the
LED).
