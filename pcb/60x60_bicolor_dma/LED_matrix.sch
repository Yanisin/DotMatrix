EESchema Schematic File Version 4
LIBS:bcl4v2-cache
EELAYER 26 0
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
Text Label 9300 1250 0    60   ~ 0
OUT_A0
Text Label 9300 1350 0    60   ~ 0
OUT_A1
Text Label 9300 1450 0    60   ~ 0
OUT_A2
Text Label 9300 1550 0    60   ~ 0
OUT_A3
Text Label 9300 1650 0    60   ~ 0
OUT_A4
Text Label 9300 1750 0    60   ~ 0
OUT_A5
Text Label 9300 1850 0    60   ~ 0
OUT_A6
Text Label 9300 1950 0    60   ~ 0
OUT_A7
Wire Wire Line
	9650 1250 9300 1250
Wire Wire Line
	9300 1350 9650 1350
Wire Wire Line
	9300 1450 9650 1450
Wire Wire Line
	9300 1550 9650 1550
Wire Wire Line
	9300 1650 9650 1650
Wire Wire Line
	9300 1750 9650 1750
Wire Wire Line
	9300 1850 9650 1850
Wire Wire Line
	9300 1950 9650 1950
$Comp
L power:GND #PWR025
U 1 1 5BD6DD18
P 7700 6150
F 0 "#PWR025" H 7700 5900 50  0001 C CNN
F 1 "GND" H 7700 6000 50  0000 C CNN
F 2 "" H 7700 6150 50  0001 C CNN
F 3 "" H 7700 6150 50  0001 C CNN
	1    7700 6150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR026
U 1 1 5BD6DD1E
P 5700 2750
F 0 "#PWR026" H 5700 2500 50  0001 C CNN
F 1 "GND" H 5700 2600 50  0000 C CNN
F 2 "" H 5700 2750 50  0001 C CNN
F 3 "" H 5700 2750 50  0001 C CNN
	1    5700 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 1350 5700 1250
Wire Wire Line
	5650 7000 6150 7000
Wire Wire Line
	5650 6800 6150 6800
Wire Wire Line
	9000 5300 8800 5300
Wire Wire Line
	9000 5400 8800 5400
Wire Wire Line
	9000 5500 8800 5500
Wire Wire Line
	9000 5600 8800 5600
Wire Wire Line
	9000 5700 8800 5700
Wire Wire Line
	9000 5800 8800 5800
Wire Wire Line
	9000 5900 8800 5900
Wire Wire Line
	9000 6000 8800 6000
Wire Wire Line
	5650 6700 6150 6700
Text Label 5650 7000 0    60   ~ 0
CAT_STROBE
Text Label 7150 5100 0    60   ~ 0
CAT_CLK
Text Label 5650 6700 0    60   ~ 0
CAT_DATA
$Comp
L power:+3.3V #PWR027
U 1 1 5BD6DD38
P 5700 1250
F 0 "#PWR027" H 5700 1100 50  0001 C CNN
F 1 "+3.3V" H 5700 1390 50  0000 C CNN
F 2 "" H 5700 1250 50  0001 C CNN
F 3 "" H 5700 1250 50  0001 C CNN
	1    5700 1250
	1    0    0    -1  
$EndComp
NoConn ~ 8150 5950
$Comp
L power:+5V #PWR028
U 1 1 5BD6DD46
P 8850 4750
F 0 "#PWR028" H 8850 4600 50  0001 C CNN
F 1 "+5V" H 8850 4890 50  0000 C CNN
F 2 "" H 8850 4750 50  0001 C CNN
F 3 "" H 8850 4750 50  0001 C CNN
	1    8850 4750
	1    0    0    -1  
$EndComp
Text Label 8950 5300 2    60   ~ 0
C8G
Text Label 8950 5400 2    60   ~ 0
C7G
Text Label 8950 5500 2    60   ~ 0
C6G
Text Label 8950 5600 2    60   ~ 0
C5G
Text Label 8950 5700 2    60   ~ 0
C4G
Text Label 8950 5800 2    60   ~ 0
C3G
Text Label 8950 5900 2    60   ~ 0
C2G
Text Label 8950 6000 2    60   ~ 0
C1G
$Comp
L power:GND #PWR029
U 1 1 5BD6E38D
P 8200 2500
F 0 "#PWR029" H 8200 2250 50  0001 C CNN
F 1 "GND" H 8200 2350 50  0000 C CNN
F 2 "" H 8200 2500 50  0001 C CNN
F 3 "" H 8200 2500 50  0001 C CNN
	1    8200 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 1550 7800 1550
Wire Wire Line
	7400 1650 7800 1650
Wire Wire Line
	7400 1750 7800 1750
Wire Wire Line
	7400 1850 7800 1850
Wire Wire Line
	7400 1950 7800 1950
Wire Wire Line
	7400 2050 7800 2050
Wire Wire Line
	7400 2150 7800 2150
Wire Wire Line
	7800 2250 7400 2250
Wire Wire Line
	8950 1550 8600 1550
Wire Wire Line
	8600 1650 8950 1650
Wire Wire Line
	8950 1750 8600 1750
Wire Wire Line
	8600 1850 8950 1850
Wire Wire Line
	8600 1950 8950 1950
Wire Wire Line
	8600 2050 8950 2050
Wire Wire Line
	8600 2150 8950 2150
Wire Wire Line
	8950 2250 8600 2250
Wire Wire Line
	8200 2450 8200 2500
Wire Wire Line
	8200 1300 8200 1350
$Comp
L power:+5V #PWR030
U 1 1 5BD6E3A7
P 8200 1300
F 0 "#PWR030" H 8200 1150 50  0001 C CNN
F 1 "+5V" H 8200 1440 50  0000 C CNN
F 2 "" H 8200 1300 50  0001 C CNN
F 3 "" H 8200 1300 50  0001 C CNN
	1    8200 1300
	1    0    0    -1  
$EndComp
Text Label 8900 1550 2    60   ~ 0
OUT_A7
Text Label 8900 1650 2    60   ~ 0
OUT_A6
Text Label 8900 1750 2    60   ~ 0
OUT_A5
Text Label 8900 1850 2    60   ~ 0
OUT_A4
Text Label 8900 1950 2    60   ~ 0
OUT_A3
Text Label 8900 2050 2    60   ~ 0
OUT_A2
Text Label 8900 2150 2    60   ~ 0
OUT_A1
Text Label 8900 2250 2    60   ~ 0
OUT_A0
Text Label 7800 1550 2    60   ~ 0
LEDS_A7
Text Label 7800 1650 2    60   ~ 0
LEDS_A6
Text Label 7800 1750 2    60   ~ 0
LEDS_A5
Text Label 7800 1850 2    60   ~ 0
LEDS_A4
Text Label 7800 1950 2    60   ~ 0
LEDS_A3
Text Label 7800 2050 2    60   ~ 0
LEDS_A2
Text Label 7800 2150 2    60   ~ 0
LEDS_A1
Text Label 7800 2250 2    60   ~ 0
LEDS_A0
Text HLabel 5650 6800 0    60   Input ~ 0
CAT_CLK
Text HLabel 5650 6700 0    60   Input ~ 0
CAT_DATA
Text HLabel 5650 7000 0    60   Input ~ 0
CAT_STROBE
$Comp
L 74xx:74HC595 U4
U 1 1 5BF3F6BF
P 5700 1950
F 0 "U4" H 5900 2500 50  0000 C CNN
F 1 "74HC595" H 6000 1300 50  0000 C CNN
F 2 "SMD_Packages:SO-16-N" H 5700 1950 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 5700 1950 50  0001 C CNN
	1    5700 1950
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0101
U 1 1 5BF5F13D
P 5100 1350
F 0 "#PWR0101" H 5100 1200 50  0001 C CNN
F 1 "+3.3V" H 5100 1490 50  0000 C CNN
F 2 "" H 5100 1350 50  0001 C CNN
F 3 "" H 5100 1350 50  0001 C CNN
	1    5100 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 1850 5100 1850
Wire Wire Line
	5100 1850 5100 1350
Wire Wire Line
	5700 2650 5700 2750
Wire Wire Line
	5300 2050 4500 2050
Wire Wire Line
	5300 1550 4550 1550
Wire Wire Line
	5300 1750 4550 1750
Wire Wire Line
	5300 2150 4500 2150
Text Label 4600 1750 0    60   ~ 0
CAT_CLK
Text Label 4600 1550 0    60   ~ 0
CAT_DATA
Text Label 4600 2050 0    60   ~ 0
CAT_STROBE
Text HLabel 4500 2150 0    60   Input ~ 0
NOE_ANODE
Text HLabel 9400 4800 2    60   Input ~ 0
NOE_CAT
$Comp
L ti:TLC5926 U6
U 1 1 5BFB7F76
P 8400 5450
F 0 "U6" H 8400 6265 50  0000 C CNN
F 1 "TLC5926" H 8400 6174 50  0000 C CNN
F 2 "Housings_SOIC:SOIC-24W_7.5x15.4mm_Pitch1.27mm" H 8400 5550 60  0001 C CNN
F 3 "" H 8400 5550 60  0000 C CNN
	1    8400 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 6000 8000 6000
Wire Wire Line
	7800 5900 8000 5900
Wire Wire Line
	7800 5800 8000 5800
Wire Wire Line
	7800 5700 8000 5700
Wire Wire Line
	7800 5600 8000 5600
Wire Wire Line
	7800 5500 8000 5500
Wire Wire Line
	7800 5400 8000 5400
Wire Wire Line
	7800 5300 8000 5300
Text Label 7850 6000 0    60   ~ 0
C8R
Text Label 7850 5900 0    60   ~ 0
C7R
Text Label 7850 5800 0    60   ~ 0
C6R
Text Label 7850 5700 0    60   ~ 0
C5R
Text Label 7850 5600 0    60   ~ 0
C4R
Text Label 7850 5500 0    60   ~ 0
C3R
Text Label 7850 5400 0    60   ~ 0
C2R
Text Label 7850 5300 0    60   ~ 0
C1R
Wire Wire Line
	8000 4900 7700 4900
Wire Wire Line
	7700 4900 7700 6150
Wire Wire Line
	8850 4750 8850 4900
Wire Wire Line
	8850 4900 8800 4900
$Comp
L bcl4v2-rescue:R R3
U 1 1 5BFD5C30
P 9250 5250
F 0 "R3" H 9320 5296 50  0000 L CNN
F 1 "R" H 9320 5205 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 9180 5250 50  0001 C CNN
F 3 "" H 9250 5250 50  0001 C CNN
	1    9250 5250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5BFD5C7E
P 9250 5450
F 0 "#PWR0102" H 9250 5200 50  0001 C CNN
F 1 "GND" H 9250 5300 50  0000 C CNN
F 2 "" H 9250 5450 50  0001 C CNN
F 3 "" H 9250 5450 50  0001 C CNN
	1    9250 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 5400 9250 5450
Wire Wire Line
	8800 5000 9250 5000
Wire Wire Line
	9250 5000 9250 5100
Text Label 6250 2450 0    60   ~ 0
CAT_DATA2
Wire Wire Line
	6100 2450 6650 2450
Text Label 7100 5000 0    60   ~ 0
CAT_DATA2
Wire Wire Line
	8000 5000 7050 5000
Wire Wire Line
	9000 4800 9000 5200
Wire Wire Line
	9000 5200 8800 5200
Wire Wire Line
	8800 5100 8950 5100
Wire Wire Line
	8950 5100 8950 4550
Text HLabel 9650 4550 2    60   Input ~ 0
CAT_DATA_OUT
Wire Wire Line
	8000 5100 7050 5100
Wire Wire Line
	8000 5200 7050 5200
Text Label 5750 6800 0    60   ~ 0
CAT_CLK
Text Label 7150 5200 0    60   ~ 0
CAT_STROBE
$Comp
L led_matrix_8x8_bicolor:LED_MATRIX_8x8_bicolor D3
U 1 1 5C028A96
P 10050 2000
F 0 "D3" H 10075 2997 60  0000 C CNN
F 1 "LED_MATRIX_8x8_bicolor" H 10075 2891 60  0000 C CNN
F 2 "kicad_libraries_new:badger_LED_MATRIX_8x8_60.2mmx60.2mm_bicolor" H 9800 2700 60  0001 C CNN
F 3 "" H 9800 2700 60  0001 C CNN
	1    10050 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10500 1950 10700 1950
Wire Wire Line
	10500 1850 10700 1850
Wire Wire Line
	10500 1750 10700 1750
Wire Wire Line
	10500 1650 10700 1650
Wire Wire Line
	10500 1550 10700 1550
Wire Wire Line
	10500 1450 10700 1450
Wire Wire Line
	10500 1350 10700 1350
Wire Wire Line
	10500 1250 10700 1250
Text Label 10550 1950 0    60   ~ 0
C8R
Text Label 10550 1850 0    60   ~ 0
C7R
Text Label 10550 1750 0    60   ~ 0
C6R
Text Label 10550 1650 0    60   ~ 0
C5R
Text Label 10550 1550 0    60   ~ 0
C4R
Text Label 10550 1450 0    60   ~ 0
C3R
Text Label 10550 1350 0    60   ~ 0
C2R
Text Label 10550 1250 0    60   ~ 0
C1R
Wire Wire Line
	10500 2750 10700 2750
Wire Wire Line
	10500 2650 10700 2650
Wire Wire Line
	10500 2550 10700 2550
Wire Wire Line
	10500 2450 10700 2450
Wire Wire Line
	10500 2350 10700 2350
Wire Wire Line
	10500 2250 10700 2250
Wire Wire Line
	10500 2150 10700 2150
Wire Wire Line
	10500 2050 10700 2050
Text Label 10550 2750 0    60   ~ 0
C8G
Text Label 10550 2650 0    60   ~ 0
C7G
Text Label 10550 2550 0    60   ~ 0
C6G
Text Label 10550 2450 0    60   ~ 0
C5G
Text Label 10550 2350 0    60   ~ 0
C4G
Text Label 10550 2250 0    60   ~ 0
C3G
Text Label 10550 2150 0    60   ~ 0
C2G
Text Label 10550 2050 0    60   ~ 0
C1G
Wire Wire Line
	6500 2250 6100 2250
Wire Wire Line
	6500 2150 6100 2150
Wire Wire Line
	6500 2050 6100 2050
Wire Wire Line
	6500 1950 6100 1950
Wire Wire Line
	6500 1850 6100 1850
Wire Wire Line
	6500 1750 6100 1750
Wire Wire Line
	6500 1650 6100 1650
Wire Wire Line
	6100 1550 6500 1550
Text Label 6100 2250 0    60   ~ 0
LEDS_A7
Text Label 6100 2150 0    60   ~ 0
LEDS_A6
Text Label 6100 2050 0    60   ~ 0
LEDS_A5
Text Label 6100 1950 0    60   ~ 0
LEDS_A4
Text Label 6100 1850 0    60   ~ 0
LEDS_A3
Text Label 6100 1750 0    60   ~ 0
LEDS_A2
Text Label 6100 1650 0    60   ~ 0
LEDS_A1
Text Label 6100 1550 0    60   ~ 0
LEDS_A0
$Comp
L Transistor_Array:TBD62783A U5
U 1 1 5C0AA9BA
P 8200 1850
F 0 "U5" H 8350 2300 50  0000 C CNN
F 1 "TBD62783A" H 8450 1300 50  0000 C CNN
F 2 "Housings_SOIC:SOIC-18W_7.5x11.6mm_Pitch1.27mm" H 8200 1300 50  0001 C CNN
F 3 "http://toshiba.semicon-storage.com/info/docget.jsp?did=30523&prodName=TBD62783APG" H 7900 2250 50  0001 C CNN
	1    8200 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 5000 4400 5000
Wire Wire Line
	4200 4900 4400 4900
Wire Wire Line
	4200 4800 4400 4800
Wire Wire Line
	4200 4700 4400 4700
Wire Wire Line
	4200 4600 4400 4600
Wire Wire Line
	4200 4500 4400 4500
Wire Wire Line
	4200 4400 4400 4400
Wire Wire Line
	4200 4300 4400 4300
Text Label 4250 5000 0    60   ~ 0
C8R
Text Label 4250 4900 0    60   ~ 0
C7R
Text Label 4250 4800 0    60   ~ 0
C6R
Text Label 4250 4700 0    60   ~ 0
C5R
Text Label 4250 4600 0    60   ~ 0
C4R
Text Label 4250 4500 0    60   ~ 0
C3R
Text Label 4250 4400 0    60   ~ 0
C2R
Text Label 4250 4300 0    60   ~ 0
C1R
Wire Wire Line
	4200 5800 4400 5800
Wire Wire Line
	4200 5700 4400 5700
Wire Wire Line
	4200 5600 4400 5600
Wire Wire Line
	4200 5500 4400 5500
Wire Wire Line
	4200 5400 4400 5400
Wire Wire Line
	4200 5300 4400 5300
Wire Wire Line
	4200 5200 4400 5200
Wire Wire Line
	4200 5100 4400 5100
Text Label 4250 5800 0    60   ~ 0
C8G
Text Label 4250 5700 0    60   ~ 0
C7G
Text Label 4250 5600 0    60   ~ 0
C6G
Text Label 4250 5500 0    60   ~ 0
C5G
Text Label 4250 5400 0    60   ~ 0
C4G
Text Label 4250 5300 0    60   ~ 0
C3G
Text Label 4250 5200 0    60   ~ 0
C2G
Text Label 4250 5100 0    60   ~ 0
C1G
$Comp
L power:GND #PWR0103
U 1 1 5C0FDA0D
P 3500 6200
F 0 "#PWR0103" H 3500 5950 50  0001 C CNN
F 1 "GND" H 3500 6050 50  0000 C CNN
F 2 "" H 3500 6200 50  0001 C CNN
F 3 "" H 3500 6200 50  0001 C CNN
	1    3500 6200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0104
U 1 1 5C0FDA34
P 3500 3900
F 0 "#PWR0104" H 3500 3750 50  0001 C CNN
F 1 "+5V" H 3500 4040 50  0000 C CNN
F 2 "" H 3500 3900 50  0001 C CNN
F 3 "" H 3500 3900 50  0001 C CNN
	1    3500 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 3900 3500 3950
Wire Wire Line
	3500 6100 3500 6150
$Comp
L badger_tlc5945pwp:TLC5945PWP U3
U 1 1 5C10DEB5
P 3500 5000
F 0 "U3" H 3100 5850 50  0000 C CNN
F 1 "TLC5945PWP" H 3800 5850 50  0000 C CNN
F 2 "kicad_libraries_new:HTSSOP-28_4.4x9.7mm_Pitch0.65mm_wo_thermal_pad" H 3525 4025 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/tlc5940.pdf" H 3100 5700 50  0001 C CNN
	1    3500 5000
	1    0    0    -1  
$EndComp
NoConn ~ 3400 6100
Wire Wire Line
	2800 4800 2300 4800
Wire Wire Line
	9000 4800 9400 4800
Text Label 9000 4800 0    60   ~ 0
NOE_CAT
Text Label 2350 4800 0    60   ~ 0
NOE_CAT
Wire Wire Line
	2800 5600 2350 5600
Wire Wire Line
	2800 5700 2350 5700
Text Label 2450 5600 0    60   ~ 0
CAT_CLK
Text Label 2350 5700 0    60   ~ 0
CAT_DATA2
Wire Wire Line
	8950 4550 9650 4550
Text Label 8950 4550 0    60   ~ 0
CAT_DATA_OUT
Text Label 2150 5800 0    60   ~ 0
CAT_DATA_OUT
Wire Wire Line
	2100 5800 2800 5800
Text Label 9050 5000 0    60   ~ 0
IREF
Text Label 2350 4400 0    60   ~ 0
IREF
$Comp
L bcl4v2-rescue:C C5
U 1 1 5C159AF6
P 4700 4950
F 0 "C5" H 4815 4996 50  0000 L CNN
F 1 "100nF" H 4815 4905 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4738 4800 50  0001 C CNN
F 3 "" H 4700 4950 50  0001 C CNN
	1    4700 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 3950 4700 3950
Wire Wire Line
	4700 3950 4700 4800
Connection ~ 3500 3950
Wire Wire Line
	3500 3950 3500 4000
Wire Wire Line
	4700 5100 4700 6050
Wire Wire Line
	4700 6050 3700 6050
Wire Wire Line
	3700 6050 3700 6150
Wire Wire Line
	3500 6150 3700 6150
Wire Wire Line
	3500 6150 3500 6200
Connection ~ 3500 6150
$Comp
L power:+5V #PWR0105
U 1 1 5C17F13A
P 2650 4050
F 0 "#PWR0105" H 2650 3900 50  0001 C CNN
F 1 "+5V" H 2650 4190 50  0000 C CNN
F 2 "" H 2650 4050 50  0001 C CNN
F 3 "" H 2650 4050 50  0001 C CNN
	1    2650 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 4500 2650 4500
Wire Wire Line
	2650 4500 2650 4050
Wire Wire Line
	2250 4400 2800 4400
Wire Wire Line
	2800 4600 1800 4600
Wire Wire Line
	2800 4300 2050 4300
Text HLabel 2050 4300 0    60   Input ~ 0
CAT_MODE
Wire Wire Line
	2800 4900 2100 4900
Text Label 2200 4900 0    60   ~ 0
CAT_STROBE
Wire Wire Line
	2800 5200 2000 5200
Text HLabel 2000 5200 0    60   Input ~ 0
CAT_ERR
Text HLabel 1200 4600 0    60   Input ~ 0
CLK_GREY
$Comp
L Oscillator:TXC-7C X1
U 1 1 5C2CFFF6
P 1400 3300
F 0 "X1" H 1741 3346 50  0000 L CNN
F 1 "TXC-7C" H 1741 3255 50  0000 L CNN
F 2 "Oscillators:Oscillator_SMD_EuroQuartz_XO53-4pin_5.0x3.2mm_HandSoldering" H 2100 2950 50  0001 C CNN
F 3 "http://www.txccorp.com/download/products/osc/7C_o.pdf" H 1300 3300 50  0001 C CNN
	1    1400 3300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0106
U 1 1 5C2D0170
P 1400 2900
F 0 "#PWR0106" H 1400 2750 50  0001 C CNN
F 1 "+3.3V" H 1400 3040 50  0000 C CNN
F 2 "" H 1400 2900 50  0001 C CNN
F 3 "" H 1400 2900 50  0001 C CNN
	1    1400 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0107
U 1 1 5C2D8262
P 1400 3650
F 0 "#PWR0107" H 1400 3400 50  0001 C CNN
F 1 "GND" H 1400 3500 50  0000 C CNN
F 2 "" H 1400 3650 50  0001 C CNN
F 3 "" H 1400 3650 50  0001 C CNN
	1    1400 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 3600 1400 3650
Wire Wire Line
	1700 3300 2650 3300
Text Label 2100 3300 0    60   ~ 0
GREY_CLK
Text Label 2050 4600 0    60   ~ 0
GREY_CLK
Wire Wire Line
	1100 3300 1000 3300
Wire Wire Line
	1000 3300 1000 3850
Wire Wire Line
	1000 4150 1000 4250
Wire Wire Line
	1000 4250 1300 4250
Wire Wire Line
	1300 4250 1300 4600
Wire Wire Line
	1300 4600 1500 4600
Wire Wire Line
	1300 4600 1200 4600
Connection ~ 1300 4600
Wire Wire Line
	1400 2900 1400 2950
Connection ~ 1400 2950
Wire Wire Line
	1400 2950 1400 3000
Wire Wire Line
	950  2950 950  3300
Wire Wire Line
	950  3300 1000 3300
Connection ~ 1000 3300
Wire Wire Line
	950  2950 1400 2950
$EndSCHEMATC