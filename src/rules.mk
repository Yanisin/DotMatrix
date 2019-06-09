##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
## Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>
## Copyright (C) 2013 Frantisek Burian <BuFran@seznam.cz>
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

# Be silent per default, but 'make V=1' will show all compiler calls.

###############################################################################
# Executables

TGT_PREFIX		?= $(ARM_NONE_EABI_DIR)/arm-none-eabi

TGT_CC		:= $(TGT_PREFIX)-gcc
TGT_CXX		:= $(TGT_PREFIX)-g++
TGT_LD		:= $(TGT_PREFIX)-gcc
TGT_AR		:= $(TGT_PREFIX)-ar
TGT_AS		:= $(TGT_PREFIX)-as
TGT_OBJCOPY		:= $(TGT_PREFIX)-objcopy
TGT_OBJDUMP		:= $(TGT_PREFIX)-objdump
TGT_GDB		:= $(TGT_PREFIX)-gdb
STFLASH		= $(shell which st-flash)
STYLECHECK	:= /checkpatch.pl
STYLECHECKFLAGS	:= --no-tree -f --terse --mailback
STYLECHECKFILES	:= $(shell find . -name '*.[ch]')

FP_FLAGS ?= -msoft-float
ARCH_FLAGS = -mthumb -mcpu=cortex-m0 $(FP_FLAGS)
ifeq ($(DEBUG),1)
TGT_OPT ?= -Og
TGT_DEFS += -DDEBUG
else
TGT_OPT ?= -Os
TGT_DEFS += -DNDEBUG
endif

###############################################################################
# Source files

TGT_OBJS = $(addsuffix .o, $(addprefix $(TGT_BUILDDIR)/, $(TGT_MODS) $(MODS)))

ifeq ($(strip $(OPENCM3_DIR)),)
$(warning Cannot find libopencm3 library in the standard search paths.)
$(error Please specify it through OPENCM3_DIR variable!)
endif

ifeq ($(V),1)
$(info Using $(OPENCM3_DIR) path to library)
endif

# Old style, assume LDSCRIPT exists
TGT_DEFS		+= -I$(OPENCM3_DIR)/include
TGT_LDFLAGS		+= -L$(OPENCM3_DIR)/lib -L..
# https://www.tablix.org/~avian/blog/archives/2012/11/gnu_linker_and_elf_program_header/
TGT_LDFLAGS += -z max-page-size=2048
TGT_LDLIBS		+= -l$(LIBNAME)

OPENCM3_SCRIPT_DIR = $(OPENCM3_DIR)/scripts
EXAMPLES_SCRIPT_DIR	= $(OPENCM3_DIR)/../scripts

###############################################################################
# C flags

TGT_CFLAGS	+= $(TGT_OPT) -std=c99
TGT_CFLAGS	+= $(ARCH_FLAGS)

###############################################################################
# C++ flags

TGT_CXXFLAGS	+= $(TGT_OPT)
TGT_CXXFLAGS	+= $(ARCH_FLAGS)

###############################################################################
# C & C++ preprocessor common flags

TGT_CPPFLAGS	+= $(TGT_DEFS)

###############################################################################
# Linker flags

TGT_LDFLAGS		+= --static -nostartfiles
TGT_LDFLAGS		+= -T$(LDSCRIPT)
TGT_LDFLAGS		+= $(ARCH_FLAGS)
TGT_LDFLAGS		+= -Wl,-Map=$(TGT_BUILDDIR)/$(*).map -Wl,--cref
ifeq ($(V),99)
TGT_LDFLAGS		+= -Wl,--print-gc-sections
endif

###############################################################################
# Used libraries

TGT_LDLIBS		+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

###############################################################################
###############################################################################
###############################################################################

.SUFFIXES: .elf .bin .hex .srec .list .map .images
.SECONDEXPANSION:
.SECONDARY:

GENERATED_BINARIES=$(addprefix $(TGT_BUILDDIR)/$(BINARY), .elf .bin .hex .srec .list .map)


# Define a helper macro for debugging make errors online
# you can type "make print-OPENCM3_DIR" and it will show you
# how that ended up being resolved by all of the included
# makefiles.
print-%:
	@echo $*=$($*)

$(TGT_BUILDDIR)/%.bin: $(TGT_BUILDDIR)/%.elf
	@printf "  OBJCOPY $(*).bin\n"
	$(Q)$(TGT_OBJCOPY) -Obinary $< $@

$(TGT_BUILDDIR)/%.hex: $(TGT_BUILDDIR)/%.elf
	@printf "  OBJCOPY $(*).hex\n"
	$(Q)$(TGT_OBJCOPY) -Oihex $< $@

$(TGT_BUILDDIR)/%.srec: $(TGT_BUILDDIR)/%.elf
	@printf "  OBJCOPY $(*).srec\n"
	$(Q)$(TGT_OBJCOPY) -Osrec $@ $<

$(TGT_BUILDDIR)/%.list: %(TGT_BUILDDIR)/%.elf
	@printf "  OBJDUMP $(*).list\n"
	$(Q)$(TGT_OBJDUMP) -S $< > $@.list

$(TGT_BUILDDIR)/%.elf %(TGT_BUILDDIR)/%.map: $(TGT_OBJS) $(LDSCRIPT)
	@printf "  LD      $(*).elf\n"
	$(Q)$(TGT_LD) $(LDFLAGS) $(TGT_LDFLAGS) $(TGT_OBJS) $(LDLIBS) $(TGT_LDLIBS) -o $@

$(TGT_BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf "  CC      $(*).c\n"
	$(Q)$(TGT_CC) $(TGT_CFLAGS) $(CFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

$(TGT_BUILDDIR)/%.o: %.cxx
	@mkdir -p $(dir $@)
	@printf "  CXX     $(*).cxx\n"
	$(Q)$(TGT_CXX) $(TGT_CXXFLAGS) $(CXXFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

$(TGT_BUILDDIR)/%.o: %.S
	@mkdir -p $(dir $@)
	@printf "  AS      $(*).S\n"
	$(Q)$(TGT_CC) $(TGT_CPPFLAGS) $(CPPFLAGS) -DASSEMBLER -o $@ -c $<

$(TGT_BUILDDIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	@printf "  CXX     $(*).cpp\n"
	$(Q)$(TGT_CXX) $(TGT_CXXFLAGS) $(CXXFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

stylecheck: $(STYLECHECKFILES:=.stylecheck)
styleclean: $(STYLECHECKFILES:=.styleclean)

# the cat is due to multithreaded nature - we like to have consistent chunks of text on the output
%.stylecheck: %
	$(Q)$(OPENCM3_SCRIPT_DIR)$(STYLECHECK) $(STYLECHECKFLAGS) $* > $*.stylecheck; \
		if [ -s $*.stylecheck ]; then \
			cat $*.stylecheck; \
		else \
			rm -f $*.stylecheck; \
		fi;

%.styleclean:
	$(Q)rm -f $*.stylecheck;

.PHONY: images stylecheck styleclean elf bin hex srec list

-include $(TGT_OBJS:.o=.d)
