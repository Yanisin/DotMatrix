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
Text Label 6900 4200 0    60   ~ 0
OUT_C1
Text Label 6900 4400 0    60   ~ 0
OUT_C3
Text Label 8100 4400 0    60   ~ 0
OUT_C4
Text Label 8100 4300 0    60   ~ 0
OUT_C5
Text Label 8100 4200 0    60   ~ 0
OUT_C6
Text Label 8100 4100 0    60   ~ 0
OUT_C7
Text Label 6900 4100 0    60   ~ 0
OUT_C0
Text Label 6900 4300 0    60   ~ 0
OUT_C2
Wire Wire Line
	6850 4200 7250 4200
Wire Wire Line
	7250 4300 6850 4300
Wire Wire Line
	7250 4400 6850 4400
Wire Wire Line
	8450 4100 8050 4100
Wire Wire Line
	8450 4200 8050 4200
Wire Wire Line
	8450 4300 8050 4300
Wire Wire Line
	8450 4400 8050 4400
Wire Wire Line
	7250 4100 6850 4100
$Comp
L bcl4v2-rescue:LED_MATRIX_8x8 D6
U 1 1 5BD6C36C
P 10550 2750
F 0 "D6" H 10050 2500 60  0000 C CNN
F 1 "LED_MATRIX_8x8" H 9850 3400 60  0000 C CNN
F 2 "kicad_libraries_new:badger_LED_MATRIX_8x8_60.2mmx60.2mm" H 9600 3250 60  0001 C CNN
F 3 "" H 9600 3250 60  0001 C CNN
	1    10550 2750
	1    0    0    -1  
$EndComp
Text Label 10350 2300 0    60   ~ 0
OUT_C1
Text Label 10350 2400 0    60   ~ 0
OUT_C2
Text Label 10350 2500 0    60   ~ 0
OUT_C3
Text Label 10350 2600 0    60   ~ 0
OUT_C4
Text Label 10350 2700 0    60   ~ 0
OUT_C5
Text Label 10350 2800 0    60   ~ 0
OUT_C6
Text Label 10350 2900 0    60   ~ 0
OUT_C7
Text Label 10350 2200 0    60   ~ 0
OUT_C0
Text Label 9100 2200 0    60   ~ 0
OUT_A0
Text Label 9100 2300 0    60   ~ 0
OUT_A1
Text Label 9100 2400 0    60   ~ 0
OUT_A2
Text Label 9100 2500 0    60   ~ 0
OUT_A3
Text Label 9100 2600 0    60   ~ 0
OUT_A4
Text Label 9100 2700 0    60   ~ 0
OUT_A5
Text Label 9100 2800 0    60   ~ 0
OUT_A6
Text Label 9100 2900 0    60   ~ 0
OUT_A7
Wire Wire Line
	10300 2300 10750 2300
Wire Wire Line
	10300 2400 10750 2400
Wire Wire Line
	10300 2500 10750 2500
Wire Wire Line
	10300 2600 10750 2600
Wire Wire Line
	10300 2700 10750 2700
Wire Wire Line
	10300 2800 10750 2800
Wire Wire Line
	10300 2900 10750 2900
Wire Wire Line
	10300 2200 10750 2200
Wire Wire Line
	9450 2200 9100 2200
Wire Wire Line
	9100 2300 9450 2300
Wire Wire Line
	9100 2400 9450 2400
Wire Wire Line
	9100 2500 9450 2500
Wire Wire Line
	9100 2600 9450 2600
Wire Wire Line
	9100 2700 9450 2700
Wire Wire Line
	9100 2800 9450 2800
Wire Wire Line
	9100 2900 9450 2900
$Comp
L bcl4v2-rescue:GND #PWR026
U 1 1 5BD6DD1E
P 6450 2950
F 0 "#PWR026" H 6450 2700 50  0001 C CNN
F 1 "GND" H 6450 2800 50  0000 C CNN
F 2 "" H 6450 2950 50  0001 C CNN
F 3 "" H 6450 2950 50  0001 C CNN
	1    6450 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 1550 6450 1450
$Comp
L bcl4v2-rescue:+3.3V #PWR027
U 1 1 5BD6DD38
P 6450 1450
F 0 "#PWR027" H 6450 1300 50  0001 C CNN
F 1 "+3.3V" H 6450 1590 50  0000 C CNN
F 2 "" H 6450 1450 50  0001 C CNN
F 3 "" H 6450 1450 50  0001 C CNN
	1    6450 1450
	1    0    0    -1  
$EndComp
$Comp
L bcl4v2-rescue:GND #PWR029
U 1 1 5BD6E38D
P 7550 2700
F 0 "#PWR029" H 7550 2450 50  0001 C CNN
F 1 "GND" H 7550 2550 50  0000 C CNN
F 2 "" H 7550 2700 50  0001 C CNN
F 3 "" H 7550 2700 50  0001 C CNN
	1    7550 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 2450 8300 2450
Wire Wire Line
	8300 2350 7950 2350
Wire Wire Line
	7950 2250 8300 2250
Wire Wire Line
	8300 2150 7950 2150
Wire Wire Line
	8300 2050 7950 2050
Wire Wire Line
	8300 1950 7950 1950
Wire Wire Line
	8300 1850 7950 1850
Wire Wire Line
	7950 1750 8300 1750
Wire Wire Line
	7550 2650 7550 2700
Wire Wire Line
	7550 1500 7550 1550
$Comp
L bcl4v2-rescue:+5V #PWR030
U 1 1 5BD6E3A7
P 7550 1500
F 0 "#PWR030" H 7550 1350 50  0001 C CNN
F 1 "+5V" H 7550 1640 50  0000 C CNN
F 2 "" H 7550 1500 50  0001 C CNN
F 3 "" H 7550 1500 50  0001 C CNN
	1    7550 1500
	1    0    0    -1  
$EndComp
Text Label 8000 2450 0    60   ~ 0
OUT_A7
Text Label 8000 2350 0    60   ~ 0
OUT_A6
Text Label 8000 2250 0    60   ~ 0
OUT_A5
Text Label 8000 2150 0    60   ~ 0
OUT_A4
Text Label 8000 2050 0    60   ~ 0
OUT_A3
Text Label 8000 1950 0    60   ~ 0
OUT_A2
Text Label 8000 1850 0    60   ~ 0
OUT_A1
Text Label 8000 1750 0    60   ~ 0
OUT_A0
$Comp
L 74xx:74HC595 U3
U 1 1 5C40190F
P 6450 2150
F 0 "U3" H 6200 2750 50  0000 C CNN
F 1 "74HC595" H 6700 2700 50  0000 C CNN
F 2 "SMD_Packages:SO-16-N" H 6450 2150 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/sn74hc595.pdf" H 6450 2150 50  0001 C CNN
	1    6450 2150
	1    0    0    -1  
$EndComp
$Comp
L bcl4v2-rescue:TLC5916-ti U5
U 1 1 5C401A25
P 7650 4050
AR Path="/5C401A25" Ref="U5"  Part="1" 
AR Path="/5BD6AFE7/5C401A25" Ref="U5"  Part="1" 
F 0 "U5" H 7400 4550 50  0000 C CNN
F 1 "TLC5916" H 7900 4550 50  0000 C CNN
F 2 "SMD_Packages:SO-16-N" H 7650 4050 60  0001 C CNN
F 3 "" H 7650 4050 60  0000 C CNN
	1    7650 4050
	1    0    0    -1  
$EndComp
$Comp
L Transistor_Array:TBD62783A U4
U 1 1 5C401B31
P 7550 2050
F 0 "U4" H 7300 2500 50  0000 C CNN
F 1 "TBD62783A" H 7800 2500 50  0000 C CNN
F 2 "kicad_lib:badger_SOIC-18W_7.5x11.6mm_Pitch1.27mm" H 7550 1500 50  0001 C CNN
F 3 "http://toshiba.semicon-storage.com/info/docget.jsp?did=30523&prodName=TBD62783APG" H 7250 2450 50  0001 C CNN
	1    7550 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 1750 7150 1750
Wire Wire Line
	7150 1850 6850 1850
Wire Wire Line
	6850 1950 7150 1950
Wire Wire Line
	7150 2050 6850 2050
Wire Wire Line
	6850 2150 7150 2150
Wire Wire Line
	7150 2250 6850 2250
Wire Wire Line
	6850 2350 7150 2350
Wire Wire Line
	6850 2450 7150 2450
Wire Wire Line
	6450 2850 6450 2950
$Comp
L bcl4v2-rescue:GND #PWR0101
U 1 1 5C44A99D
P 7050 3550
F 0 "#PWR0101" H 7050 3300 50  0001 C CNN
F 1 "GND" H 7050 3400 50  0000 C CNN
F 2 "" H 7050 3550 50  0001 C CNN
F 3 "" H 7050 3550 50  0001 C CNN
	1    7050 3550
	1    0    0    -1  
$EndComp
$Comp
L bcl4v2-rescue:+3.3V #PWR0102
U 1 1 5C44A9B0
P 8200 3600
F 0 "#PWR0102" H 8200 3450 50  0001 C CNN
F 1 "+3.3V" H 8200 3740 50  0000 C CNN
F 2 "" H 8200 3600 50  0001 C CNN
F 3 "" H 8200 3600 50  0001 C CNN
	1    8200 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 2650 6900 2650
Wire Wire Line
	6900 2650 6900 3800
Wire Wire Line
	6900 3800 7250 3800
Wire Wire Line
	7250 3700 7200 3700
Wire Wire Line
	7200 3700 7200 3450
Wire Wire Line
	7200 3450 7050 3450
Wire Wire Line
	7050 3450 7050 3550
Wire Wire Line
	8050 3700 8200 3700
Wire Wire Line
	8200 3700 8200 3600
Wire Wire Line
	8050 3800 8800 3800
Wire Wire Line
	8800 3800 8800 4000
$Comp
L bcl4v2-rescue:R R3
U 1 1 5C455FDE
P 8800 4150
F 0 "R3" H 8870 4196 50  0000 L CNN
F 1 "1k" H 8870 4105 50  0000 L CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 8730 4150 50  0001 C CNN
F 3 "" H 8800 4150 50  0001 C CNN
	1    8800 4150
	1    0    0    -1  
$EndComp
$Comp
L bcl4v2-rescue:GND #PWR0103
U 1 1 5C456060
P 8800 4400
F 0 "#PWR0103" H 8800 4150 50  0001 C CNN
F 1 "GND" H 8800 4250 50  0000 C CNN
F 2 "" H 8800 4400 50  0001 C CNN
F 3 "" H 8800 4400 50  0001 C CNN
	1    8800 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 4300 8800 4400
NoConn ~ 8050 3900
Wire Wire Line
	7250 3900 6400 3900
Text Label 6500 3900 0    60   ~ 0
DISP_CLK
Wire Wire Line
	7250 4000 6400 4000
Wire Wire Line
	8050 4000 8500 4000
Text Label 6500 4000 0    60   ~ 0
DISP_STROBE
Text Label 8150 4000 0    60   ~ 0
CAT_NOE
Text Label 3150 5100 0    60   ~ 0
CAT_NOE
Text Label 3150 4550 0    60   ~ 0
DISP_CLK
Text Label 3050 4750 0    60   ~ 0
DISP_STROBE
Wire Wire Line
	6050 1750 5450 1750
Text Label 5450 1750 0    60   ~ 0
DISP_DATA
Text Label 3100 4650 0    60   ~ 0
DISP_DATA
Wire Wire Line
	6050 1950 5450 1950
Text Label 5450 1950 0    60   ~ 0
DISP_CLK
Wire Wire Line
	6050 2050 6000 2050
Wire Wire Line
	6000 2050 6000 1550
$Comp
L bcl4v2-rescue:+3.3V #PWR0104
U 1 1 5C47043D
P 6000 1550
F 0 "#PWR0104" H 6000 1400 50  0001 C CNN
F 1 "+3.3V" H 6000 1690 50  0000 C CNN
F 2 "" H 6000 1550 50  0001 C CNN
F 3 "" H 6000 1550 50  0001 C CNN
	1    6000 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 2250 5450 2250
Wire Wire Line
	6050 2350 5450 2350
Text Label 5450 2250 0    60   ~ 0
DISP_STROBE
Text Label 5450 2350 0    60   ~ 0
AN_NOE
Text Label 3100 5250 0    60   ~ 0
AN_NOE
Text HLabel 3000 4650 0    60   Input ~ 0
DISP_DATA
Text HLabel 3000 4750 0    60   Input ~ 0
DISP_STROBE
Text HLabel 3000 4550 0    60   Input ~ 0
DISP_CLK
Text HLabel 3050 5100 0    60   Input ~ 0
CAT_NOE
Text HLabel 3000 5250 0    60   Input ~ 0
AN_NOE
Wire Wire Line
	3000 4550 3600 4550
Wire Wire Line
	3000 4650 3600 4650
Wire Wire Line
	3050 5100 3550 5100
Wire Wire Line
	3000 5250 3550 5250
Wire Wire Line
	3000 4750 3600 4750
$EndSCHEMATC
