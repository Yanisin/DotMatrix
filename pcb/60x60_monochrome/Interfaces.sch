EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:badger_ir2301
LIBS:badger_mosfet_sot669
LIBS:badger_zxct1110
LIBS:stm32
LIBS:74xgxx
LIBS:ac-dc
LIBS:actel
LIBS:Altera
LIBS:analog_devices
LIBS:bbd
LIBS:brooktre
LIBS:cmos_ieee
LIBS:dc-dc
LIBS:diode
LIBS:elec-unifil
LIBS:ESD_Protection
LIBS:ftdi
LIBS:gennum
LIBS:hc11
LIBS:ir
LIBS:Lattice
LIBS:maxim
LIBS:microchip_dspic33dsc
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic32mcu
LIBS:motor_drivers
LIBS:msp430
LIBS:nordicsemi
LIBS:nxp_armmcu
LIBS:onsemi
LIBS:Oscillators
LIBS:powerint
LIBS:Power_Management
LIBS:pspice
LIBS:references
LIBS:relays
LIBS:rfcom
LIBS:sensors
LIBS:silabs
LIBS:stm8
LIBS:supertex
LIBS:switches
LIBS:transf
LIBS:ttl_ieee
LIBS:video
LIBS:Worldsemi
LIBS:Xicor
LIBS:Zilog
LIBS:badger_dualmosfet_DMC2038LVT
LIBS:wt51822-s4at
LIBS:badger_resonator
LIBS:LED_MATRIX_ysm-2388crgbc
LIBS:badger_dm163_LED_DRIVER
LIBS:badger_led_matrix_8x8
LIBS:badger_m54564fp
LIBS:badger_npic6c595
LIBS:bcl4v2-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 3850 2250 3    60   ~ 0
I2C_SCL
$Comp
L VCC #PWR30
U 1 1 5BD6FE4D
P 3150 2200
F 0 "#PWR30" H 3150 2050 50  0001 C CNN
F 1 "VCC" H 3150 2350 50  0000 C CNN
F 2 "" H 3150 2200 50  0001 C CNN
F 3 "" H 3150 2200 50  0001 C CNN
	1    3150 2200
	0    -1   1    0   
$EndComp
Text Label 3650 2250 3    60   ~ 0
I2C_SDA
Text Label 4050 2250 3    60   ~ 0
UART_RX4
Text Label 3450 2300 3    60   ~ 0
UART_TX4
$Comp
L Conn_01x05 J1
U 1 1 5BD6FE56
P 3450 1900
F 0 "J1" H 3450 2200 50  0000 C CNN
F 1 "Conn_01x05" H 3450 1600 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 3450 1900 50  0001 C CNN
F 3 "" H 3450 1900 50  0001 C CNN
	1    3450 1900
	0    -1   -1   0   
$EndComp
$Comp
L Conn_01x05 J2
U 1 1 5BD6FE5D
P 4050 1900
F 0 "J2" H 4050 2200 50  0000 C CNN
F 1 "Conn_01x05" H 4050 1600 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 4050 1900 50  0001 C CNN
F 3 "" H 4050 1900 50  0001 C CNN
	1    4050 1900
	0    1    -1   0   
$EndComp
Text Label 3550 2350 3    60   ~ 0
IN1P
Text Label 3950 2350 3    60   ~ 0
BTN0
Wire Wire Line
	3850 2100 3850 2650
Wire Wire Line
	4050 2100 4050 2650
Wire Wire Line
	4250 2100 4250 2150
Wire Wire Line
	3250 2150 4300 2150
Wire Wire Line
	3250 2100 3250 2150
Connection ~ 4250 2150
Wire Wire Line
	4150 2200 4150 2100
Wire Wire Line
	3150 2200 4150 2200
Wire Wire Line
	3350 2100 3350 2200
Connection ~ 3350 2200
Wire Wire Line
	3450 2100 3450 2650
Wire Wire Line
	3650 2100 3650 2650
Wire Wire Line
	3950 2100 3950 2600
Wire Wire Line
	3550 2100 3550 2650
$Comp
L GND #PWR32
U 1 1 5BD6FE74
P 4300 2150
F 0 "#PWR32" H 4300 1900 50  0001 C CNN
F 1 "GND" H 4300 2000 50  0000 C CNN
F 2 "" H 4300 2150 50  0001 C CNN
F 3 "" H 4300 2150 50  0001 C CNN
	1    4300 2150
	0    -1   1    0   
$EndComp
Text Label 4000 4950 1    60   ~ 0
I2C_SCL
$Comp
L VCC #PWR31
U 1 1 5BD7123B
P 3300 5000
F 0 "#PWR31" H 3300 4850 50  0001 C CNN
F 1 "VCC" H 3300 5150 50  0000 C CNN
F 2 "" H 3300 5000 50  0001 C CNN
F 3 "" H 3300 5000 50  0001 C CNN
	1    3300 5000
	0    -1   -1   0   
$EndComp
Text Label 3800 4950 1    60   ~ 0
I2C_SDA
Text Label 4200 4950 1    60   ~ 0
UART_TX2
Text Label 3600 4950 1    60   ~ 0
UART_RX2
$Comp
L Conn_01x05 J5
U 1 1 5BD71244
P 3600 5300
F 0 "J5" H 3600 5600 50  0000 C CNN
F 1 "Conn_01x05" H 3600 5000 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 3600 5300 50  0001 C CNN
F 3 "" H 3600 5300 50  0001 C CNN
	1    3600 5300
	0    -1   1    0   
$EndComp
$Comp
L Conn_01x05 J4
U 1 1 5BD7124B
P 4200 5300
F 0 "J4" H 4200 5600 50  0000 C CNN
F 1 "Conn_01x05" H 4200 5000 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 4200 5300 50  0001 C CNN
F 3 "" H 4200 5300 50  0001 C CNN
	1    4200 5300
	0    1    1    0   
$EndComp
Text Label 4100 4900 1    60   ~ 0
IN0P
Text Label 3700 4900 1    60   ~ 0
BTN1
Wire Wire Line
	4000 5100 4000 4550
Wire Wire Line
	4200 5100 4200 4550
Wire Wire Line
	4400 5100 4400 5050
Wire Wire Line
	3400 5050 4450 5050
Wire Wire Line
	3400 5100 3400 5050
Connection ~ 4400 5050
Wire Wire Line
	4300 5000 4300 5100
Wire Wire Line
	3300 5000 4300 5000
Wire Wire Line
	3500 5100 3500 5000
Connection ~ 3500 5000
Wire Wire Line
	3600 5100 3600 4550
Wire Wire Line
	3800 5100 3800 4550
Wire Wire Line
	3700 5100 3700 4600
Wire Wire Line
	4100 5100 4100 4550
$Comp
L GND #PWR33
U 1 1 5BD71262
P 4450 5050
F 0 "#PWR33" H 4450 4800 50  0001 C CNN
F 1 "GND" H 4450 4900 50  0000 C CNN
F 2 "" H 4450 5050 50  0001 C CNN
F 3 "" H 4450 5050 50  0001 C CNN
	1    4450 5050
	0    -1   -1   0   
$EndComp
Text Label 5250 3350 2    60   ~ 0
UART_TX1
Text Label 5250 3950 2    60   ~ 0
UART_RX1
Text Label 5250 3550 2    60   ~ 0
I2C_SCL
$Comp
L VCC #PWR34
U 1 1 5BD71F25
P 5300 4250
F 0 "#PWR34" H 5300 4100 50  0001 C CNN
F 1 "VCC" H 5300 4400 50  0000 C CNN
F 2 "" H 5300 4250 50  0001 C CNN
F 3 "" H 5300 4250 50  0001 C CNN
	1    5300 4250
	-1   0    0    1   
$EndComp
Text Label 5250 3750 2    60   ~ 0
I2C_SDA
Text Label 2350 3300 0    60   ~ 0
UART_RX3
Text Label 2400 3900 0    60   ~ 0
UART_TX3
Text Label 2350 3500 0    60   ~ 0
I2C_SCL
$Comp
L VCC #PWR29
U 1 1 5BD71F2F
P 2300 4200
F 0 "#PWR29" H 2300 4050 50  0001 C CNN
F 1 "VCC" H 2300 4350 50  0000 C CNN
F 2 "" H 2300 4200 50  0001 C CNN
F 3 "" H 2300 4200 50  0001 C CNN
	1    2300 4200
	1    0    0    1   
$EndComp
Text Label 2350 3700 0    60   ~ 0
I2C_SDA
$Comp
L Conn_01x05 J9
U 1 1 5BD71F36
P 5600 3950
F 0 "J9" H 5600 4250 50  0000 C CNN
F 1 "Conn_01x05" H 5600 3650 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 5600 3950 50  0001 C CNN
F 3 "" H 5600 3950 50  0001 C CNN
	1    5600 3950
	1    0    0    1   
$EndComp
$Comp
L Conn_01x05 J8
U 1 1 5BD71F3D
P 5600 3350
F 0 "J8" H 5600 3650 50  0000 C CNN
F 1 "Conn_01x05" H 5600 3050 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 5600 3350 50  0001 C CNN
F 3 "" H 5600 3350 50  0001 C CNN
	1    5600 3350
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x05 J6
U 1 1 5BD71F44
P 2000 3900
F 0 "J6" H 2000 4200 50  0000 C CNN
F 1 "Conn_01x05" H 2000 3600 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 2000 3900 50  0001 C CNN
F 3 "" H 2000 3900 50  0001 C CNN
	1    2000 3900
	-1   0    0    1   
$EndComp
$Comp
L Conn_01x05 J7
U 1 1 5BD71F4B
P 2000 3300
F 0 "J7" H 2000 3600 50  0000 C CNN
F 1 "Conn_01x05" H 2000 3000 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 2000 3300 50  0001 C CNN
F 3 "" H 2000 3300 50  0001 C CNN
	1    2000 3300
	-1   0    0    -1  
$EndComp
Text Label 5200 3450 2    60   ~ 0
IN0P
Text Label 2400 3800 0    60   ~ 0
IN1P
Text Label 2400 3400 0    60   ~ 0
IN3
Text Label 5200 3850 2    60   ~ 0
IN2
Wire Wire Line
	5400 3550 4850 3550
Wire Wire Line
	5400 3350 4850 3350
Wire Wire Line
	5400 3150 5350 3150
Wire Wire Line
	5350 4150 5350 3100
Wire Wire Line
	5400 4150 5350 4150
Connection ~ 5350 3150
Wire Wire Line
	5300 3250 5400 3250
Wire Wire Line
	5300 4250 5300 3250
Wire Wire Line
	5400 4050 5300 4050
Connection ~ 5300 4050
Wire Wire Line
	5400 3950 4850 3950
Wire Wire Line
	5400 3750 4850 3750
Wire Wire Line
	2200 3500 2750 3500
Wire Wire Line
	2200 3300 2750 3300
Wire Wire Line
	2200 3100 2250 3100
Wire Wire Line
	2250 4100 2250 3050
Wire Wire Line
	2200 4100 2250 4100
Connection ~ 2250 3100
Wire Wire Line
	2300 3200 2200 3200
Wire Wire Line
	2300 4200 2300 3200
Wire Wire Line
	2200 4000 2300 4000
Connection ~ 2300 4000
Wire Wire Line
	2200 3900 2750 3900
Wire Wire Line
	2200 3700 2750 3700
Wire Wire Line
	2200 3800 2700 3800
Wire Wire Line
	2200 3400 2750 3400
Wire Wire Line
	5400 3850 4850 3850
Wire Wire Line
	5400 3450 4900 3450
Wire Wire Line
	4900 3450 4900 3500
$Comp
L GND #PWR35
U 1 1 5BD71F73
P 5350 3100
F 0 "#PWR35" H 5350 2850 50  0001 C CNN
F 1 "GND" H 5350 2950 50  0000 C CNN
F 2 "" H 5350 3100 50  0001 C CNN
F 3 "" H 5350 3100 50  0001 C CNN
	1    5350 3100
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR28
U 1 1 5BD71F79
P 2250 3050
F 0 "#PWR28" H 2250 2800 50  0001 C CNN
F 1 "GND" H 2250 2900 50  0000 C CNN
F 2 "" H 2250 3050 50  0001 C CNN
F 3 "" H 2250 3050 50  0001 C CNN
	1    2250 3050
	1    0    0    1   
$EndComp
$Comp
L Conn_01x01_Female J13
U 1 1 5BD72A87
P 9650 5850
F 0 "J13" H 9650 5950 50  0000 C CNN
F 1 "Conn_01x01_Female" H 9650 5750 50  0000 C CNN
F 2 "badger_footprints_kicad_4x:badger_mount_M3" H 9650 5850 50  0001 C CNN
F 3 "" H 9650 5850 50  0001 C CNN
	1    9650 5850
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x01_Female J12
U 1 1 5BD72A8E
P 9650 5600
F 0 "J12" H 9650 5700 50  0000 C CNN
F 1 "Conn_01x01_Female" H 9650 5500 50  0000 C CNN
F 2 "badger_footprints_kicad_4x:badger_mount_M3" H 9650 5600 50  0001 C CNN
F 3 "" H 9650 5600 50  0001 C CNN
	1    9650 5600
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x01_Female J11
U 1 1 5BD72A95
P 9650 5350
F 0 "J11" H 9650 5450 50  0000 C CNN
F 1 "Conn_01x01_Female" H 9650 5250 50  0000 C CNN
F 2 "badger_footprints_kicad_4x:badger_mount_M3" H 9650 5350 50  0001 C CNN
F 3 "" H 9650 5350 50  0001 C CNN
	1    9650 5350
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x01_Female J10
U 1 1 5BD72A9C
P 9650 5150
F 0 "J10" H 9650 5250 50  0000 C CNN
F 1 "Conn_01x01_Female" H 9650 5050 50  0000 C CNN
F 2 "badger_footprints_kicad_4x:badger_mount_M3" H 9650 5150 50  0001 C CNN
F 3 "" H 9650 5150 50  0001 C CNN
	1    9650 5150
	1    0    0    -1  
$EndComp
NoConn ~ 9450 5150
NoConn ~ 9450 5350
NoConn ~ 9450 5600
NoConn ~ 9450 5850
Text Label 9500 3800 0    60   ~ 0
NRST
Text Label 9500 3900 0    60   ~ 0
SWDIO
Text Label 9500 4000 0    60   ~ 0
SWCLK
$Comp
L USB_OTG J3
U 1 1 5BD73B46
P 9300 1850
F 0 "J3" H 9100 2300 50  0000 L CNN
F 1 "USB_OTG" H 9100 2200 50  0000 L CNN
F 2 "Connect:USB_Micro-B" H 9450 1800 50  0001 C CNN
F 3 "" H 9450 1800 50  0001 C CNN
	1    9300 1850
	1    0    0    -1  
$EndComp
NoConn ~ 9600 2050
$Comp
L D D3
U 1 1 5BD73B4E
P 9800 1650
F 0 "D3" H 9800 1750 50  0000 C CNN
F 1 "Shottky" H 9800 1550 50  0000 C CNN
F 2 "Diodes_SMD:D_SOD-123" H 9800 1650 50  0001 C CNN
F 3 "" H 9800 1650 50  0000 C CNN
	1    9800 1650
	-1   0    0    1   
$EndComp
Text Label 9750 1850 0    60   ~ 0
USB_DP
Text Label 9750 1950 0    60   ~ 0
USB_DM
$Comp
L +5V #PWR37
U 1 1 5BD73B57
P 10000 1600
F 0 "#PWR37" H 10000 1450 50  0001 C CNN
F 1 "+5V" H 10000 1740 50  0000 C CNN
F 2 "" H 10000 1600 50  0001 C CNN
F 3 "" H 10000 1600 50  0001 C CNN
	1    10000 1600
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x02 J14
U 1 1 5BD73B5D
P 9300 3300
F 0 "J14" H 9300 3400 50  0000 C CNN
F 1 "Conn_01x02" H 9300 3100 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x02_2.54mm_angled_handsolder" H 9300 3300 50  0001 C CNN
F 3 "" H 9300 3300 50  0001 C CNN
	1    9300 3300
	-1   0    0    1   
$EndComp
Text Label 9550 3300 0    60   ~ 0
BOOT0
$Comp
L Conn_01x04 J15
U 1 1 5BD73B65
P 9250 4000
F 0 "J15" H 9250 4200 50  0000 C CNN
F 1 "Conn_01x04" H 9250 3700 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x04_2.54mm_angled_handsolder" H 9250 4000 50  0001 C CNN
F 3 "" H 9250 4000 50  0001 C CNN
	1    9250 4000
	-1   0    0    1   
$EndComp
$Comp
L +3.3V #PWR38
U 1 1 5BD73B6C
P 9600 3100
F 0 "#PWR38" H 9600 2950 50  0001 C CNN
F 1 "+3.3V" H 9600 3240 50  0000 C CNN
F 2 "" H 9600 3100 50  0001 C CNN
F 3 "" H 9600 3100 50  0001 C CNN
	1    9600 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 3900 9850 3900
Wire Wire Line
	9500 4150 9500 4100
Wire Wire Line
	9500 4100 9450 4100
Wire Wire Line
	9450 4000 9850 4000
Wire Wire Line
	9450 3800 9850 3800
Wire Wire Line
	9200 2250 9200 2300
Wire Wire Line
	9200 2300 9300 2300
Wire Wire Line
	9300 2250 9300 2350
Connection ~ 9300 2300
Wire Wire Line
	9600 1850 10100 1850
Wire Wire Line
	9600 1950 10100 1950
Wire Wire Line
	9600 1650 9650 1650
Wire Wire Line
	10000 1600 10000 1650
Wire Wire Line
	10000 1650 9950 1650
Wire Wire Line
	9500 3300 9850 3300
Wire Wire Line
	9500 3200 9600 3200
Wire Wire Line
	9600 3200 9600 3100
$Comp
L GND #PWR39
U 1 1 5BD73B83
P 9500 4150
F 0 "#PWR39" H 9500 3900 50  0001 C CNN
F 1 "GND" H 9500 4000 50  0000 C CNN
F 2 "" H 9500 4150 50  0001 C CNN
F 3 "" H 9500 4150 50  0001 C CNN
	1    9500 4150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR36
U 1 1 5BD73B89
P 9300 2350
F 0 "#PWR36" H 9300 2100 50  0001 C CNN
F 1 "GND" H 9300 2200 50  0000 C CNN
F 2 "" H 9300 2350 50  0001 C CNN
F 3 "" H 9300 2350 50  0001 C CNN
	1    9300 2350
	1    0    0    -1  
$EndComp
$EndSCHEMATC
