ifeq ($(wildcard ../settings.mk),)
$(error Please create settings.mk. Use settings.mk.example as a guide)
endif

include ../settings.mk

ALL = build/$(BINARY).elf

LIBNAME = opencm3_stm32f0
DEFS += -DSTM32F0 -D$(HW_VERSION)

FP_FLAGS ?= -msoft-float
ARCH_FLAGS = -mthumb -mcpu=cortex-m0 $(FP_FLAGS)
TGT_BUILDDIR = build
SIM_BUILDDIR = build-sim

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

LDFLAGS += -Wl,--gc-sections

###############################################################################
# Build sim & target

ifneq ($(V),1)
Q := @
NULL := 2>/dev/null
endif

ALL := $(TGT_BUILDDIR)/$(BINARY).elf
ifeq ($(SIM), 1)
ALL += $(SIM_BUILDDIR)/$(BINARY)
endif

.PHONY: all clean prg prg_usb gdb
.SUFFIXES:

all: $(ALL)

clean:
	rm -rf $(SIM_BUILDDIR)
	rm -rf $(TGT_BUILDDIR)

ifeq ($(SIM), 1)
include ../rules.sim.mk
endif
include ../rules.mk

###############################################################################
# Programming & debugging
#

TGT_RESULT = $(TGT_BUILDDIR)/$(BINARY)

prg: $(TGT_RESULT).elf
	@echo programming...
	$(OOCD) -f ../openocd_stm32_orange.cfg -c"program $(TGT_RESULT).elf verify reset exit"

# FIXME what is the meaning of the number behind colon?
prg_usb: $(TGT_RESULT).bin
	@sudo dfu-util -a 0 -D $(TGT_RESULT).bin -s 0x08000000:131072

gdb:  $(TGT_RESULT).elf
		$(TGT_GDB) -ex 'target remote | openocd -f ../openocd_stm32_orange.cfg -c "gdb_port pipe; log_output openocd.log"' \
			-ex 'monitor reset halt' \
			-ex 'b common_main' -ex 'c' \
			$(TFT_RESULT).elf

