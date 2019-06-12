BINARY = full

DEFS := -DCONSOLE_PRINTF
TGT_CPPFLAGS = -Ichibi
SIM_CFLAGS = -Wno-redundant-decls
SIM_CPPFLAGS = -Isim/chibi
SIM ?= 1
USE_CHIBI_OS ?= 1
CPPFLAGS = -I.

TGT_MODS = \
	chibi/hal_st \
	chibi/reset \
	chibi/init \
	hw/led \
	hw/disp \
	hw/disp_spi \
	hw/cdcacm \
	hw/console \
	hw/spi_dma \
	hw/cell_id \
	hw/buttons_hw \
	hw/board \
	hw/i2c

SIM_MODS = \
	sim/chibi/chcore \
	sim/board \
	sim/main \
	sim/sim \
	sim/led \
	sim/disp \
	sim/console \
	sim/i2c

MODS += \
	main \
	hw/usart_buffered \
	hw/buttons \
	util/rand \
	util/topo \
	util/vector \
	util/icons \
	util/message_queue \
	util/byte_queue \
	util/stack_usage \
	util/simple_printf \
	util/initializers \
	applets/applet \
	applets/chooser_applet \
	applets/topo_applet \
	applets/life

include ../common.mk
