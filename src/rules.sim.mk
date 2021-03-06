SIM_PREFIX ?=
SIM_CC := $(HOST_PREFIX)gcc
SIM_CXX := $(HOST_PREFIX)g++
SIM_LD := $(HOST_PREFIX)ld
SIM_AR := $(HOST_PREFIX)ar
SIM_AS := $(HOST_PREFIX)as
SIM_OBJS = $(addsuffix .o, $(addprefix $(SIM_BUILDDIR)/, $(SIM_MODS) $(MODS)))

.PHONY: clean proto

SIM_DEFS += -DDEBUG -DSIM
SIM_CFLAGS += -std=gnu11
SIM_CFLAGS += -Og -m32
SIM_CPPFLAGS += $(SIM_DEFS) $(DEFS)
SIM_LDFLAGS += -lpthread -lrt -T ../sim.ld -m32
SIM_ASFLAGS += -m32

$(SIM_BUILDDIR)/$(BINARY): $(SIM_OBJS)
	@mkdir -p $(dir $@)
	@printf "  SIMLD   $(BINARY)\n"
	$(Q)$(SIM_CC) $(SIM_OBJS) -o $@ $(LDFLAGS) $(SIM_LDFLAGS)

$(SIM_BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf "  SIMCC   $(*).c\n"
	$(Q)$(SIM_CC) $(SIM_CFLAGS) $(CFLAGS) $(SIM_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

$(SIM_BUILDDIR)/%.o: %.cxx
	@mkdir -p $(dir $@)
	@printf "  SIMCXX  $(*).cxx\n"
	$(Q)$(SIM_CC) $(SIM_CXXLAGS) $(CXXFLAGS) $(SIM_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

$(SIM_BUILDDIR)/%.o: %.S
	@mkdir -p $(dir $@)
	@printf "  SIMAS   $(*).S\n"
	$(Q)$(SIM_CC) $(SIM_ASFLAGS) $(SIM_CPPFLAGS) $(CPPFLAGS) -DASSEMBLER -o $@ -c $<

$(SIM_OBJS): sim/proto_defs.h

sim/proto_defs.h: ../simgui/proto_defs.py
	@printf "  PROTO\n"
	$(Q)python3 $(ROOT)/simgui/proto_defs.py > $@

-include $(SIM_OBJS:.o=.d)
