BINARY = main

DEFS += -DCONSOLE_PRINTF
SIM ?= 1

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
	buttons_hw

SIM_MODS = \
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

#OBJS += wordclock.o rtc.o

include ../common.mk
