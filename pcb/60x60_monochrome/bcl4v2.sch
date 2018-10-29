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
Sheet 1 3
Title ""
Date "24 mar 2016"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 4250 1550 0    60   ~ 0
NRST
Text Label 4200 1750 0    60   ~ 0
BOOT0
Text Label 10600 4050 0    60   ~ 0
SWCLK
Text Label 10600 3950 0    60   ~ 0
SWDIO
Text Label 1150 3650 1    60   ~ 0
LED
Text Label 4300 2450 0    60   ~ 0
LED
$Comp
L +5V #PWR1
U 1 1 5B2FC143
P 700 900
F 0 "#PWR1" H 700 750 50  0001 C CNN
F 1 "+5V" H 700 1040 50  0000 C CNN
F 2 "" H 700 900 50  0001 C CNN
F 3 "" H 700 900 50  0001 C CNN
	1    700  900 
	1    0    0    -1  
$EndComp
$Comp
L LED D5
U 1 1 5B30C71A
P 1150 2950
F 0 "D5" H 1150 3050 50  0000 C CNN
F 1 "LED" H 1150 2850 50  0000 C CNN
F 2 "LEDs:LED_0805_HandSoldering" H 1150 2950 50  0001 C CNN
F 3 "" H 1150 2950 50  0001 C CNN
	1    1150 2950
	0    -1   -1   0   
$EndComp
$Comp
L R R5
U 1 1 5B30C89A
P 1150 3300
F 0 "R5" V 1230 3300 50  0000 C CNN
F 1 "1k" V 1150 3300 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 1080 3300 50  0001 C CNN
F 3 "" H 1150 3300 50  0001 C CNN
	1    1150 3300
	1    0    0    -1  
$EndComp
Text Label 10550 3050 0    60   ~ 0
IN0
Text Label 10850 3650 2    60   ~ 0
UART_RX1
Text Label 10850 3550 2    60   ~ 0
UART_TX1
Text Label 10950 2950 2    60   ~ 0
UART_RX2
Text Label 10900 2850 2    60   ~ 0
UART_TX2
Text Label 4550 3750 2    60   ~ 0
UART_RX3
Text Label 4550 3650 2    60   ~ 0
UART_TX3
Text Label 10950 2750 2    60   ~ 0
UART_RX4
Text Label 10950 2650 2    60   ~ 0
UART_TX4
$Comp
L STM32F070CBTx U2
U 1 1 5B7C507C
P 7500 2850
F 0 "U2" H 4700 4475 50  0000 L BNN
F 1 "STM32F070CBTx" H 10300 4475 50  0000 R BNN
F 2 "kicad_libraries_new:badger_LQFP-48_7x7mm_Pitch0.5mm_handsolder" H 10300 4425 50  0001 R TNN
F 3 "" H 7500 2850 50  0001 C CNN
	1    7500 2850
	1    0    0    -1  
$EndComp
Text Label 10450 3250 0    60   ~ 0
LEDS_A0
Text Label 10450 3350 0    60   ~ 0
LEDS_A1
Text Label 10450 3450 0    60   ~ 0
LEDS_A2
Text Label 4200 2650 0    60   ~ 0
LEDS_A3
Text Label 4200 2750 0    60   ~ 0
LEDS_A4
Text Label 4200 3050 0    60   ~ 0
LEDS_A5
Text Label 4200 3250 0    60   ~ 0
LEDS_A6
Text Label 4200 3350 0    60   ~ 0
LEDS_A7
Text Label 10550 3750 0    60   ~ 0
USB_DM
Text Label 10550 3850 0    60   ~ 0
USB_DP
Text Label 4200 3950 0    60   ~ 0
I2C_SCL
Text Label 4200 4050 0    60   ~ 0
I2C_SDA
Text Label 10550 3150 0    60   ~ 0
IN1
Text Label 1750 6800 0    60   ~ 0
IN1
$Comp
L D D2
U 1 1 5B7CBB46
P 1350 6800
F 0 "D2" H 1350 6900 50  0000 C CNN
F 1 "Shottky" H 1350 6700 50  0000 C CNN
F 2 "LEDs:LED_0805_HandSoldering" H 1350 6800 50  0001 C CNN
F 3 "" H 1350 6800 50  0000 C CNN
	1    1350 6800
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 5B7CBB52
P 1350 7050
F 0 "R4" V 1430 7050 50  0000 C CNN
F 1 "10k" V 1350 7050 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 1280 7050 50  0001 C CNN
F 3 "" H 1350 7050 50  0000 C CNN
	1    1350 7050
	0    -1   -1   0   
$EndComp
Text Notes 850  6650 0    50   ~ 0
Shottky level shifter,\ntogether with the internal pull-up.\nR to GND is for analog in possibility\nin that case replace the D with resistor\nto create a voltage divider\n
$Comp
L D D4
U 1 1 5B7CC26D
P 2550 850
F 0 "D4" H 2550 950 50  0000 C CNN
F 1 "Shottky" H 2550 750 50  0000 C CNN
F 2 "LEDs:LED_0805_HandSoldering" H 2550 850 50  0001 C CNN
F 3 "" H 2550 850 50  0000 C CNN
	1    2550 850 
	-1   0    0    1   
$EndComp
$Comp
L +5V #PWR15
U 1 1 5B7CC45E
P 2750 800
F 0 "#PWR15" H 2750 650 50  0001 C CNN
F 1 "+5V" H 2750 940 50  0000 C CNN
F 2 "" H 2750 800 50  0001 C CNN
F 3 "" H 2750 800 50  0001 C CNN
	1    2750 800 
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR6
U 1 1 5B7CDF17
P 1800 900
F 0 "#PWR6" H 1800 750 50  0001 C CNN
F 1 "+3.3V" H 1800 1040 50  0000 C CNN
F 2 "" H 1800 900 50  0001 C CNN
F 3 "" H 1800 900 50  0001 C CNN
	1    1800 900 
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR4
U 1 1 5B7CDF67
P 1150 2700
F 0 "#PWR4" H 1150 2550 50  0001 C CNN
F 1 "+3.3V" H 1150 2840 50  0000 C CNN
F 2 "" H 1150 2700 50  0001 C CNN
F 3 "" H 1150 2700 50  0001 C CNN
	1    1150 2700
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR20
U 1 1 5B7CDFA3
P 7300 1050
F 0 "#PWR20" H 7300 900 50  0001 C CNN
F 1 "+3.3V" H 7300 1190 50  0000 C CNN
F 2 "" H 7300 1050 50  0001 C CNN
F 3 "" H 7300 1050 50  0001 C CNN
	1    7300 1050
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR10
U 1 1 5B7CE578
P 2350 800
F 0 "#PWR10" H 2350 650 50  0001 C CNN
F 1 "VCC" H 2350 950 50  0000 C CNN
F 2 "" H 2350 800 50  0001 C CNN
F 3 "" H 2350 800 50  0001 C CNN
	1    2350 800 
	1    0    0    -1  
$EndComp
Text Label 4100 3150 0    60   ~ 0
CAT_DATA
Text Label 4100 2950 0    60   ~ 0
CAT_CLK
Text Label 10500 4150 0    60   ~ 0
CAT_STROBE
Text Label 4200 3550 0    60   ~ 0
I2C_SCL
Text Label 4200 3450 0    60   ~ 0
I2C_SDA
$Comp
L APE8865N-33-HF-3 U1
U 1 1 5B8162CA
P 1150 950
F 0 "U1" H 1000 1075 50  0000 C CNN
F 1 "DS9166A-23" H 1150 1075 50  0000 L CNN
F 2 "kicad_libraries_new:badger_SOT-23_handsolder" H 1150 1175 50  0001 C CIN
F 3 "" H 1150 950 50  0001 C CNN
	1    1150 950 
	1    0    0    -1  
$EndComp
$Comp
L SW_Push SW1
U 1 1 5B895ABD
P 2200 3250
F 0 "SW1" H 2250 3350 50  0000 L CNN
F 1 "SW_Push" H 2200 3190 50  0000 C CNN
F 2 "kicad_libraries_new:badger_smd_button_KSS231G" H 2200 3450 50  0001 C CNN
F 3 "" H 2200 3450 50  0001 C CNN
	1    2200 3250
	0    1    1    0   
$EndComp
$Comp
L C C2
U 1 1 5B895B32
P 2000 3250
F 0 "C2" H 2025 3350 50  0000 L CNN
F 1 "100n" H 2025 3150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2038 3100 50  0001 C CNN
F 3 "" H 2000 3250 50  0001 C CNN
	1    2000 3250
	-1   0    0    1   
$EndComp
$Comp
L R R14
U 1 1 5B896672
P 2000 2800
F 0 "R14" V 2080 2800 50  0000 C CNN
F 1 "220k" V 2000 2800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 1930 2800 50  0001 C CNN
F 3 "" H 2000 2800 50  0001 C CNN
	1    2000 2800
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR8
U 1 1 5B8967C1
P 2000 2600
F 0 "#PWR8" H 2000 2450 50  0001 C CNN
F 1 "+3.3V" H 2000 2740 50  0000 C CNN
F 2 "" H 2000 2600 50  0001 C CNN
F 3 "" H 2000 2600 50  0001 C CNN
	1    2000 2600
	1    0    0    -1  
$EndComp
Text Label 2250 3000 0    60   ~ 0
BTN0
Text Label 4300 2250 0    60   ~ 0
BTN0
$Comp
L Resonator Y1
U 1 1 5BD2F92C
P 5050 6800
F 0 "Y1" H 5050 7025 50  0000 C CNN
F 1 "Resonator" H 5050 6950 50  0000 C CNN
F 2 "kicad_libraries_new:badger_CSTCE_G" H 5025 6800 50  0001 C CNN
F 3 "" H 5025 6800 50  0001 C CNN
	1    5050 6800
	1    0    0    -1  
$EndComp
Text Label 4150 1950 0    60   ~ 0
OSC_IN
Text Label 4150 2050 0    60   ~ 0
OSC_OUT
Text Label 4450 6800 0    60   ~ 0
OSC_IN
Text Label 5250 6800 0    60   ~ 0
OSC_OUT
Text Notes 650  4550 0    60   ~ 0
consider adding capacitors for stabilsing the led flashing\n(something around 500uF should do the job for total current of 200mA, 8 rows and 25Hz)
Text Notes 750  4750 0    60   ~ 0
add shotky/jumper/polyfuse so the VCC rail can be powered from the usb
Text Label 950  6800 0    60   ~ 0
IN1P
Text Label 3100 6800 0    60   ~ 0
IN0
$Comp
L D D1
U 1 1 5BD53305
P 2700 6800
F 0 "D1" H 2700 6900 50  0000 C CNN
F 1 "Shottky" H 2700 6700 50  0000 C CNN
F 2 "LEDs:LED_0805_HandSoldering" H 2700 6800 50  0001 C CNN
F 3 "" H 2700 6800 50  0000 C CNN
	1    2700 6800
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5BD5330B
P 2700 7050
F 0 "R1" V 2780 7050 50  0000 C CNN
F 1 "10k" V 2700 7050 50  0000 C CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" V 2630 7050 50  0001 C CNN
F 3 "" H 2700 7050 50  0000 C CNN
	1    2700 7050
	0    -1   -1   0   
$EndComp
Text Label 2300 6800 0    60   ~ 0
IN0P
Text Label 4250 4150 0    60   ~ 0
IN3
Text Label 4250 3850 0    60   ~ 0
IN2
$Comp
L SW_Push SW2
U 1 1 5BD4A8D8
P 2900 3250
F 0 "SW2" H 2950 3350 50  0000 L CNN
F 1 "SW_Push" H 2900 3190 50  0000 C CNN
F 2 "kicad_libraries_new:badger_smd_button_KSS231G" H 2900 3450 50  0001 C CNN
F 3 "" H 2900 3450 50  0001 C CNN
	1    2900 3250
	0    1    1    0   
$EndComp
$Comp
L C C4
U 1 1 5BD4A8DE
P 2700 3250
F 0 "C4" H 2725 3350 50  0000 L CNN
F 1 "100n" H 2725 3150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2738 3100 50  0001 C CNN
F 3 "" H 2700 3250 50  0001 C CNN
	1    2700 3250
	-1   0    0    1   
$EndComp
$Comp
L R R2
U 1 1 5BD4A8EA
P 2700 2800
F 0 "R2" V 2780 2800 50  0000 C CNN
F 1 "220k" V 2700 2800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 2630 2800 50  0001 C CNN
F 3 "" H 2700 2800 50  0001 C CNN
	1    2700 2800
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR13
U 1 1 5BD4A8F0
P 2700 2600
F 0 "#PWR13" H 2700 2450 50  0001 C CNN
F 1 "+3.3V" H 2700 2740 50  0000 C CNN
F 2 "" H 2700 2600 50  0001 C CNN
F 3 "" H 2700 2600 50  0001 C CNN
	1    2700 2600
	1    0    0    -1  
$EndComp
Text Label 2950 3000 0    60   ~ 0
BTN1
Text Label 4300 2350 0    60   ~ 0
BTN1
Text Notes 600  5050 0    60   ~ 0
Consider to replace the shottky shifter on inputs with bidirectional (mosfet)
Text Notes 750  4900 0    60   ~ 0
if possible consider btn to be attached to ADC_IN\n
$Comp
L R R15
U 1 1 5BD5B5FD
P 3850 1950
F 0 "R15" V 3930 1950 50  0000 C CNN
F 1 "10k" V 3850 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3780 1950 50  0001 C CNN
F 3 "" H 3850 1950 50  0001 C CNN
	1    3850 1950
	1    0    0    -1  
$EndComp
Connection ~ 7300 4650
Wire Wire Line
	7300 4550 7300 4700
Wire Wire Line
	7400 4550 7400 4650
Wire Wire Line
	7300 4650 7600 4650
Wire Wire Line
	4050 1550 4600 1550
Wire Wire Line
	3850 1750 4600 1750
Wire Wire Line
	1150 3100 1150 3150
Wire Wire Line
	1150 3450 1150 3700
Wire Wire Line
	4600 1950 4050 1950
Wire Wire Line
	4600 2050 4050 2050
Wire Wire Line
	1150 2700 1150 2800
Wire Wire Line
	700  900  700  1000
Connection ~ 700  950 
Wire Wire Line
	700  1300 700  1350
Wire Wire Line
	1800 1350 1800 1400
Connection ~ 1800 950 
Wire Wire Line
	1800 950  1450 950 
Wire Wire Line
	7500 4650 7500 4550
Connection ~ 7400 4650
Wire Wire Line
	7600 4650 7600 4550
Connection ~ 7500 4650
Wire Wire Line
	7300 1050 7300 1150
Wire Wire Line
	7300 1100 7700 1100
Wire Wire Line
	7700 1100 7700 1150
Connection ~ 7300 1100
Wire Wire Line
	7600 1150 7600 1100
Connection ~ 7600 1100
Wire Wire Line
	7500 1150 7500 1100
Connection ~ 7500 1100
Wire Wire Line
	7400 1150 7400 1100
Connection ~ 7400 1100
Wire Wire Line
	10400 2650 11050 2650
Wire Wire Line
	10400 2750 11050 2750
Wire Wire Line
	10400 2850 11050 2850
Wire Wire Line
	10400 2950 11050 2950
Wire Wire Line
	10400 3050 11050 3050
Wire Wire Line
	10400 3150 11050 3150
Wire Wire Line
	10400 3250 11050 3250
Wire Wire Line
	10400 3350 11050 3350
Wire Wire Line
	10400 3450 11050 3450
Wire Wire Line
	10400 3550 11050 3550
Wire Wire Line
	10400 3650 11050 3650
Wire Wire Line
	10400 3750 11050 3750
Wire Wire Line
	10400 3850 11050 3850
Wire Wire Line
	10400 3950 11050 3950
Wire Wire Line
	10400 4050 11050 4050
Wire Wire Line
	10400 4150 11050 4150
Wire Wire Line
	4600 2250 4100 2250
Wire Wire Line
	4600 2350 4100 2350
Wire Wire Line
	4600 2450 4100 2450
Wire Wire Line
	4600 2650 4100 2650
Wire Wire Line
	4600 2750 4100 2750
Wire Wire Line
	4600 2850 4100 2850
Wire Wire Line
	4600 2950 4100 2950
Wire Wire Line
	4600 3050 4100 3050
Wire Wire Line
	4600 3150 4100 3150
Wire Wire Line
	4600 3250 4100 3250
Wire Wire Line
	4600 3350 4100 3350
Wire Wire Line
	4600 3450 4100 3450
Wire Wire Line
	4600 3550 4100 3550
Wire Wire Line
	4600 3650 4100 3650
Wire Wire Line
	4600 3750 4100 3750
Wire Wire Line
	4600 3850 4100 3850
Wire Wire Line
	4600 3950 4100 3950
Wire Wire Line
	4600 4050 4100 4050
Wire Wire Line
	4600 4150 4100 4150
Wire Wire Line
	950  6800 1200 6800
Wire Wire Line
	1500 6800 1950 6800
Connection ~ 1550 6800
Wire Wire Line
	2750 800  2750 850 
Wire Wire Line
	2750 850  2700 850 
Wire Wire Line
	2350 800  2350 850 
Wire Wire Line
	2350 850  2400 850 
Wire Wire Line
	700  950  850  950 
Wire Wire Line
	1800 900  1800 1050
Wire Wire Line
	1150 1250 1150 1350
Wire Wire Line
	2200 3450 2200 3500
Wire Wire Line
	2200 3500 2000 3500
Wire Wire Line
	2000 3400 2000 3550
Connection ~ 2000 3500
Wire Wire Line
	2000 2950 2000 3100
Wire Wire Line
	2200 3050 2200 3000
Wire Wire Line
	2000 3000 2350 3000
Connection ~ 2000 3000
Connection ~ 2200 3000
Wire Wire Line
	2000 2600 2000 2650
Wire Wire Line
	5050 7000 5050 7050
Wire Wire Line
	4900 6800 4400 6800
Wire Wire Line
	5200 6800 5650 6800
Wire Wire Line
	1550 6800 1550 7050
Wire Wire Line
	1550 7050 1500 7050
Wire Wire Line
	1200 7050 1150 7050
Wire Wire Line
	1150 7050 1150 7100
Wire Wire Line
	2300 6800 2550 6800
Wire Wire Line
	2850 6800 3300 6800
Connection ~ 2900 6800
Wire Wire Line
	2900 6800 2900 7050
Wire Wire Line
	2900 7050 2850 7050
Wire Wire Line
	2550 7050 2500 7050
Wire Wire Line
	2500 7050 2500 7100
Wire Wire Line
	2900 3450 2900 3500
Wire Wire Line
	2900 3500 2700 3500
Wire Wire Line
	2700 3400 2700 3550
Connection ~ 2700 3500
Wire Wire Line
	2700 2950 2700 3100
Wire Wire Line
	2900 3050 2900 3000
Wire Wire Line
	2700 3000 3050 3000
Connection ~ 2700 3000
Connection ~ 2900 3000
Wire Wire Line
	2700 2600 2700 2650
Wire Wire Line
	3850 2100 3850 2200
Wire Wire Line
	3850 1750 3850 1800
$Comp
L C C5
U 1 1 5BD5BDA4
P 3900 1550
F 0 "C5" H 3925 1650 50  0000 L CNN
F 1 "100n" H 3925 1450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3938 1400 50  0001 C CNN
F 3 "" H 3900 1550 50  0001 C CNN
	1    3900 1550
	0    1    1    0   
$EndComp
Wire Wire Line
	3750 1550 3700 1550
Wire Wire Line
	3700 1550 3700 1650
Text Notes 700  5200 0    60   ~ 0
consider adding all those power capacitors suggested by AN4325\n(still the "big ones" missing)
$Comp
L GND #PWR2
U 1 1 5BD581B6
P 700 1350
F 0 "#PWR2" H 700 1100 50  0001 C CNN
F 1 "GND" H 700 1200 50  0000 C CNN
F 2 "" H 700 1350 50  0001 C CNN
F 3 "" H 700 1350 50  0001 C CNN
	1    700  1350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR3
U 1 1 5BD5842F
P 1150 1350
F 0 "#PWR3" H 1150 1100 50  0001 C CNN
F 1 "GND" H 1150 1200 50  0000 C CNN
F 2 "" H 1150 1350 50  0001 C CNN
F 3 "" H 1150 1350 50  0001 C CNN
	1    1150 1350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR7
U 1 1 5BD584A7
P 1800 1400
F 0 "#PWR7" H 1800 1150 50  0001 C CNN
F 1 "GND" H 1800 1250 50  0000 C CNN
F 2 "" H 1800 1400 50  0001 C CNN
F 3 "" H 1800 1400 50  0001 C CNN
	1    1800 1400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR14
U 1 1 5BD585E9
P 2700 3550
F 0 "#PWR14" H 2700 3300 50  0001 C CNN
F 1 "GND" H 2700 3400 50  0000 C CNN
F 2 "" H 2700 3550 50  0001 C CNN
F 3 "" H 2700 3550 50  0001 C CNN
	1    2700 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR9
U 1 1 5BD58661
P 2000 3550
F 0 "#PWR9" H 2000 3300 50  0001 C CNN
F 1 "GND" H 2000 3400 50  0000 C CNN
F 2 "" H 2000 3550 50  0001 C CNN
F 3 "" H 2000 3550 50  0001 C CNN
	1    2000 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR5
U 1 1 5BD58C74
P 1150 7100
F 0 "#PWR5" H 1150 6850 50  0001 C CNN
F 1 "GND" H 1150 6950 50  0000 C CNN
F 2 "" H 1150 7100 50  0001 C CNN
F 3 "" H 1150 7100 50  0001 C CNN
	1    1150 7100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR12
U 1 1 5BD58D1F
P 2500 7100
F 0 "#PWR12" H 2500 6850 50  0001 C CNN
F 1 "GND" H 2500 6950 50  0000 C CNN
F 2 "" H 2500 7100 50  0001 C CNN
F 3 "" H 2500 7100 50  0001 C CNN
	1    2500 7100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR21
U 1 1 5BD59223
P 7300 4700
F 0 "#PWR21" H 7300 4450 50  0001 C CNN
F 1 "GND" H 7300 4550 50  0000 C CNN
F 2 "" H 7300 4700 50  0001 C CNN
F 3 "" H 7300 4700 50  0001 C CNN
	1    7300 4700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR18
U 1 1 5BD5945E
P 3850 2200
F 0 "#PWR18" H 3850 1950 50  0001 C CNN
F 1 "GND" H 3850 2050 50  0000 C CNN
F 2 "" H 3850 2200 50  0001 C CNN
F 3 "" H 3850 2200 50  0001 C CNN
	1    3850 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR17
U 1 1 5BD59509
P 3700 1650
F 0 "#PWR17" H 3700 1400 50  0001 C CNN
F 1 "GND" H 3700 1500 50  0000 C CNN
F 2 "" H 3700 1650 50  0001 C CNN
F 3 "" H 3700 1650 50  0001 C CNN
	1    3700 1650
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 5BD97D7C
P 2350 1500
F 0 "C6" H 2375 1600 50  0000 L CNN
F 1 "100n" H 2375 1400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 2388 1350 50  0001 C CNN
F 3 "" H 2350 1500 50  0001 C CNN
F 4 "4" H 2350 1500 60  0001 C CNN "Price"
	1    2350 1500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR11
U 1 1 5BD9FBAA
P 2350 1750
F 0 "#PWR11" H 2350 1500 50  0001 C CNN
F 1 "GND" H 2350 1600 50  0000 C CNN
F 2 "" H 2350 1750 50  0001 C CNN
F 3 "" H 2350 1750 50  0001 C CNN
	1    2350 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 1650 2350 1750
Wire Wire Line
	2350 1700 3100 1700
Wire Wire Line
	2600 1700 2600 1650
Connection ~ 2350 1700
Wire Wire Line
	2850 1700 2850 1650
Connection ~ 2600 1700
Wire Wire Line
	3100 1700 3100 1650
Connection ~ 2850 1700
Wire Wire Line
	2350 1350 2350 1300
Wire Wire Line
	2350 1300 3100 1300
Wire Wire Line
	3100 1250 3100 1350
Wire Wire Line
	2850 1350 2850 1300
Connection ~ 2850 1300
Wire Wire Line
	2600 1350 2600 1300
Connection ~ 2600 1300
$Comp
L +3.3V #PWR16
U 1 1 5BDA051E
P 3100 1250
F 0 "#PWR16" H 3100 1100 50  0001 C CNN
F 1 "+3.3V" H 3100 1390 50  0000 C CNN
F 2 "" H 3100 1250 50  0001 C CNN
F 3 "" H 3100 1250 50  0001 C CNN
	1    3100 1250
	1    0    0    -1  
$EndComp
Connection ~ 3100 1300
$Comp
L C C7
U 1 1 5BDA0A5A
P 2600 1500
F 0 "C7" H 2625 1600 50  0000 L CNN
F 1 "100n" H 2625 1400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 2638 1350 50  0001 C CNN
F 3 "" H 2600 1500 50  0001 C CNN
	1    2600 1500
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 5BDA0B44
P 2850 1500
F 0 "C8" H 2875 1600 50  0000 L CNN
F 1 "100n" H 2875 1400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 2888 1350 50  0001 C CNN
F 3 "" H 2850 1500 50  0001 C CNN
	1    2850 1500
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 5BDA0CEE
P 3100 1500
F 0 "C9" H 3125 1600 50  0000 L CNN
F 1 "100n" H 3125 1400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603_HandSoldering" H 3138 1350 50  0001 C CNN
F 3 "" H 3100 1500 50  0001 C CNN
	1    3100 1500
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 5BD5C3C9
P 700 1150
F 0 "C1" H 725 1250 50  0000 L CNN
F 1 "2u2" H 725 1050 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 738 1000 50  0001 C CNN
F 3 "" H 700 1150 50  0001 C CNN
	1    700  1150
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5BD5C62F
P 1800 1200
F 0 "C3" H 1825 1300 50  0000 L CNN
F 1 "2u2" H 1825 1100 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1838 1050 50  0001 C CNN
F 3 "" H 1800 1200 50  0001 C CNN
	1    1800 1200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR19
U 1 1 5BD5CCBD
P 5050 7050
F 0 "#PWR19" H 5050 6800 50  0001 C CNN
F 1 "GND" H 5050 6900 50  0000 C CNN
F 2 "" H 5050 7050 50  0001 C CNN
F 3 "" H 5050 7050 50  0001 C CNN
	1    5050 7050
	1    0    0    -1  
$EndComp
Text Notes 700  5500 0    60   ~ 0
consider exchanging pin 4 and 5 on the headers\nso the ones  expected to connect are adjecent
NoConn ~ 4100 2850
$Sheet
S 10000 5800 700  400 
U 5BD6AFE7
F0 "LED_matrix" 60
F1 "LED_matrix.sch" 60
$EndSheet
$Sheet
S 9050 5800 700  400 
U 5BD6EB23
F0 "Interfaces" 60
F1 "Interfaces.sch" 60
$EndSheet
Text Notes 850  5700 0    60   ~ 0
consider larger crystal and external resistor read AN2864
$EndSCHEMATC
