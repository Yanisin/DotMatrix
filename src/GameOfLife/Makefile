TOPDIR = $(shell pwd)

BINARY = GameOfLife
#HW_VERSION = "HW_38x38_rev_0_1"
#HW_VERSION = "HW_60x60_rev_0_2"
#HW_VERSION = "HW_38x38_rev_0_3"
#HW_VERSION = "HW_60x60_rev_0_4"
HW_VERSION = "HW_60x60_rev_0_5"

DEFS += -D$(HW_VERSION)
#DEFS += -DCONSOLE_PRINTF
MODS= buttons led disp disp_spi ticker rand usart_buffered spi_dma applet
MODS += cdcacm console main
#MODS += wordclock rtc
MODS += life

include ../common.mk
