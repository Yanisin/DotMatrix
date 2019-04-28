##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
##
## This library is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.
##

TOPDIR = $(shell pwd)

BINARY = main
#HW_VERSION = "HW_38x38_rev_0_1"
#HW_VERSION = "HW_60x60_rev_0_2"
#HW_VERSION = "HW_38x38_rev_0_3"
#HW_VERSION = "HW_60x60_rev_0_4"
HW_VERSION = "HW_60x60_rev_0_5"

DEFS += -D$(HW_VERSION)
#DEFS += -DCONSOLE_PRINTF
OBJS= buttons.o led.o disp.o disp_spi.o ticker.o rand.o usart_buffered.o spi_dma.o applet.o
OBJS += cdcacm.o console.o
#OBJS += wordclock.o rtc.o
OBJS += life.o

LDSCRIPT = ../stm32f070.ld
CFLAGS += -Werror

include ../Makefile.include.f0
include ../makefile.badger
