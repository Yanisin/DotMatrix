BINARY = full

DEFS := -DCONSOLE_PRINTF
TGT_CPPFLAGS = -Ichibi
SIM_CFLAGS = -Wno-redundant-decls
SIM_CPPFLAGS = -Isim/chibi
SIM ?= 1
USE_CHIBI_OS ?= 1

TGT_MODS = \
	led \
	disp disp_spi \
	ticker \
	cdcacm \
	console \
	common_gpio \
	spi_dma \
	int \
	cell_id \
	buttons_hw \
	board

SIM_MODS = \
	sim/board \
	sim/chibi/hal_lld \
	sim/main \
	sim/sim_proto \
	sim/led \
	sim/ticker \
	sim/disp \
	sim/console \
	sim/common_gpio \
	sim/int \
	sim/buttons_hw

MODS += \
	rand \
	main \
	applet \
	life \
	usart_buffered \
	topo \
	vector \
	icons \
	chooser_applet \
	topo_applet \
	buttons

include ../common.mk
