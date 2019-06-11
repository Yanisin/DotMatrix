ifeq ($(wildcard ../settings.mk),)
$(error Please create settings.mk. Use settings.mk.example as a guide)
endif

ROOT=$(CURDIR)/..

include $(ROOT)/settings.mk

ALL = build/$(BINARY).elf

LIBNAME = opencm3_stm32f0
DEFS += -DSTM32F0 -D$(HW_VERSION)

TGT_BUILDDIR = build
SIM_BUILDDIR = build-sim
SIM ?= 0
TGT ?= 1
DEBUG ?= 0
SUDO ?= sudo
NO_WAIT ?= 0

###############################################################################
# Common C Flags

CFLAGS += -ggdb3
CFLAGS += -Wextra -Wshadow -Wimplicit-function-declaration
CFLAGS += -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
CFLAGS += -fno-common -ffunction-sections -fdata-sections

CXXFLAGS += -Wextra -Wshadow -Wredundant-decls  -Weffc++
CXXFLAGS += -fno-common -ffunction-sections -fdata-sections

CPPFLAGS += -MD
CPPFLAGS += -Wall -Wundef
CPPFLAGS += $(DEFS)
CPPFLAGS += -I ../hwdefs

LDFLAGS += -Wl,--gc-sections

###############################################################################
# ChibiOS
#

# Suck out list of files to build from chibi Makefiles
ifeq ($(USE_CHIBI_OS),1)
ALLKERNSRC :=
ALLINC :=
ALLXASMSRC :=

ifeq ($(TGT),1)
include $(CHIBIOS)/os/common/ports/ARMCMx/compilers/GCC/mk/port_v6m.mk
CHIBI_TGT_MODS := $(patsubst $(CHIBIOS)/%,%,$(ALLCSRC:.c=) $(ALLXASMSRC:.S=))
CHIBI_TGT_CPPFLAGS := $(addprefix -I, $(ALLINC))
TGT_MODS += $(CHIBI_TGT_MODS)
TGT_CPPFLAGS += $(CHIBI_TGT_CPPFLAGS)
ALLCSRC :=
ALLINC :=
ALLXASMSRC :=
endif

ifeq ($(SIM),1)
# Currentl we have our port in-tree, but the Makefile mechanism is left here
# just in case
CHIBI_SIM_MODS := $(patsubst $(CHIBIOS)/%,%,$(ALLCSRC:.c=) $(ALLXASMSRC:.S=))
CHIBI_SIM_CPPFLAGS := $(addprefix -I, $(ALLINC))
SIM_MODS += $(CHIBI_SIM_MODS)
SIM_CPPFLAGS += $(CHIBI_SIM_CPPFLAGS)
ALLCSRC :=
ALLINC :=
ALLXASMSRC :=
endif

include $(CHIBIOS)/os/rt/rt.mk
include $(CHIBIOS)/os/license/license.mk
include $(CHIBIOS)/os/hal/osal/rt/osal.mk

# We remove the CHIBIOS prefix and use VPATH instead
VPATH := $(CHIBIOS)
MODS += $(patsubst $(CHIBIOS)/%,%,$(ALLCSRC:.c=) $(ALLXASMSRC:.S=))
CPPFLAGS += $(addprefix -I, $(ALLINC))
# And build just queue from hal
DEFS += -DPORT_IGNORE_GCC_VERSION_CHECK
endif

###############################################################################
# Build sim & target
#

ifneq ($(V),1)
Q := @
NULL := 2>/dev/null
endif

ifeq ($(NO_BOOTLOADER), 1)
LDSCRIPT = ../stm32f070-preboot.ld
else
LDSCRIPT = ../stm32f070.ld
endif

TGT_RESULT = $(TGT_BUILDDIR)/$(BINARY)

ALL :=
ifeq ($(TGT), 1)
ALL += $(TGT_RESULT).elf $(TGT_RESULT).bin
endif
ifeq ($(SIM), 1)
ALL += $(SIM_BUILDDIR)/$(BINARY)
endif

.PHONY: all clean prg prg_usb prg_bl gdb
.SUFFIXES:

ifeq ($(TGT), 1)
all: $(ALL)
	size $(TGT_RESULT).elf
else
all: $(ALL)
endif

clean:
	rm -rf $(SIM_BUILDDIR)
	rm -rf $(TGT_BUILDDIR)

ifeq ($(SIM), 1)
include $(ROOT)/rules.sim.mk
endif

ifeq ($(TGT), 1)
include $(ROOT)/rules.mk
endif

###############################################################################
# Programming & debugging
#


prg: $(TGT_RESULT).elf
	@echo programming...
	$(OOCD) -f ../openocd_stm32_orange.cfg -c"program $(TGT_RESULT).elf verify reset exit"

# FIXME what is the meaning of the number behind colon?

ifeq ($(NO_BOOTLOADER), 1)
# Program using the ST link bootloader
prg_bl: $(TGT_RESULT).bin
	@$(SUDO) dfu-util -a 0 -D $(TGT_RESULT).bin -s 0x08000000:131072

prg_usb:
	$(error Program is built without bootloader support)

else
prg_bl: $(TGT_RESULT).bin
	@$(SUDO) dfu-util -a 0 -D $(TGT_RESULT).bin -s 0x08004000:131072
# program using our prebooter (DDFU)
prg_usb: $(TGT_RESULT).bin
	
	@# We "arm" sudo using this command
	@$(SUDO) echo Waiting for the bootloader to connect, please restart the device
ifeq ($(NO_WAIT),0)
	@../wait_for_dfu
endif
	@$(SUDO) dfu-util -D $(TGT_RESULT).bin
endif

gdb:  $(TGT_RESULT).elf
		$(TGT_GDB) -ex 'target remote | openocd -f ../openocd_stm32_orange.cfg \
			-c "stm32f0x.cpu configure -rtos auto; gdb_port pipe; log_output openocd.log"' \
			-ex 'monitor reset halt' \
			-ex 'b main' -ex 'c' \
			$(TGT_RESULT).elf

sizes: $(TGT_RESULT).elf
	nm --print-size --size-sort $< -l -t d | awk -e '$$3 == "b" || $$3 == "d"'

###############################################################################
# Help
#
help:
	@echo "all (default)   - build the $(TGT_RESULT).elf and $(TGT_RESULT).bin files"
	@echo "prg             - program the binary using OpenOCD and STlink"
	@echo "prg_bl          - program the binary using ST's bootloader"
	@echo "prg_usb         - program all cells over USB"
	@echo "gdb             - debug the target over OpenOCD"

