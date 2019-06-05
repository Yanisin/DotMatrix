BINARY = full

DEFS := -DCONSOLE_PRINTF
TGT_CPPFLAGS = -Ichibi
SIM_CFLAGS = -Wno-redundant-decls
SIM_CPPFLAGS = -Isim/chibi
SIM ?= 1
USE_CHIBI_OS ?= 1

TGT_MODS = \
	chibi/hal_st \
	chibi/reset \
	chibi/init \
	led \
	disp disp_spi \
	cdcacm \
	console \
	common_gpio \
	spi_dma \
	cell_id \
	buttons_hw \
	board

SIM_MODS = \
	sim/chibi/chcore \
	sim/board \
	sim/main \
	sim/sim \
	sim/led \
	sim/disp \
	sim/console \
	sim/common_gpio \
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
