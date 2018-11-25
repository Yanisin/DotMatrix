EESchema Schematic File Version 4
LIBS:bcl4v3-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L dk_PMIC-LED-Drivers:TLC5916IN U4
U 1 1 5BE49B21
P 9600 3400
F 0 "U4" H 9350 3650 60  0000 C CNN
F 1 "TLC5916ID" H 9900 3650 60  0000 C CNN
F 2 "Package_SO:TSSOP-16_4.4x5mm_P0.65mm" H 9800 3600 60  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/tlc5916.pdf" H 9800 3700 60  0001 L CNN
F 4 "296-24383-5-ND" H 9800 3800 60  0001 L CNN "Digi-Key_PN"
F 5 "TLC5916ID" H 9800 3900 60  0001 L CNN "MPN"
F 6 "Integrated Circuits (ICs)" H 9800 4000 60  0001 L CNN "Category"
F 7 "PMIC - LED Drivers" H 9800 4100 60  0001 L CNN "Family"
F 8 "http://www.ti.com/lit/ds/symlink/tlc5916.pdf" H 9800 4200 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/texas-instruments/TLC5916IN/296-24383-5-ND/1906409" H 9800 4300 60  0001 L CNN "DK_Detail_Page"
F 10 "IC LED DRIVER LINEAR 120MA 16SOIC" H 9800 4400 60  0001 L CNN "Description"
F 11 "Texas Instruments" H 9800 4500 60  0001 L CNN "Manufacturer"
F 12 "Active" H 9800 4600 60  0001 L CNN "Status"
	1    9600 3400
	1    0    0    -1  
$EndComp
$Comp
L dk_Logic-Shift-Registers:74HC595D_118 U2
U 1 1 5BE49D9C
P 8150 1550
F 0 "U2" H 7900 1600 60  0000 C CNN
F 1 "74HC595D_118" H 8600 1600 60  0000 C CNN
F 2 "Package_SO:TSSOP-16_4.4x5mm_P0.65mm" H 8350 1750 60  0001 L CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/74HC_HCT595.pdf" H 8350 1850 60  0001 L CNN
F 4 "1727-2821-1-ND" H 8350 1950 60  0001 L CNN "Digi-Key_PN"
F 5 "74HC595D,118" H 8350 2050 60  0001 L CNN "MPN"
F 6 "Integrated Circuits (ICs)" H 8350 2150 60  0001 L CNN "Category"
F 7 "Logic - Shift Registers" H 8350 2250 60  0001 L CNN "Family"
F 8 "https://assets.nexperia.com/documents/data-sheet/74HC_HCT595.pdf" H 8350 2350 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/nexperia-usa-inc/74HC595D,118/1727-2821-1-ND/763394" H 8350 2450 60  0001 L CNN "DK_Detail_Page"
F 10 "IC SHIFT REGISTER 8BIT 16SOIC" H 8350 2550 60  0001 L CNN "Description"
F 11 "Nexperia USA Inc." H 8350 2650 60  0001 L CNN "Manufacturer"
F 12 "Active" H 8350 2750 60  0001 L CNN "Status"
	1    8150 1550
	1    0    0    -1  
$EndComp
$Comp
L Transistor_Array:TBD62783A U3
U 1 1 5BE4A090
P 9600 1950
F 0 "U3" H 9350 2400 50  0000 C CNN
F 1 "TBD62783A" H 9900 2400 50  0000 C CNN
F 2 "Package_SO:SSOP-18_4.4x6.5mm_P0.65mm" H 9600 1400 50  0001 C CNN
F 3 "http://toshiba.semicon-storage.com/info/docget.jsp?did=30523&prodName=TBD62783APG" H 9300 2350 50  0001 C CNN
	1    9600 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 1650 9200 1650
Wire Wire Line
	8650 1750 9200 1750
Wire Wire Line
	8650 1850 9200 1850
Wire Wire Line
	8650 1950 9200 1950
Wire Wire Line
	8650 2050 9200 2050
Wire Wire Line
	8650 2150 9200 2150
Wire Wire Line
	8650 2250 9200 2250
Wire Wire Line
	8650 2350 9200 2350
Wire Wire Line
	8650 2450 9150 2450
Wire Wire Line
	9150 2450 9150 3800
Wire Wire Line
	9150 3800 9200 3800
Wire Wire Line
	10000 3300 10250 3300
Wire Wire Line
	10000 3400 10250 3400
Wire Wire Line
	10000 3500 10250 3500
Wire Wire Line
	10000 3600 10250 3600
Wire Wire Line
	10000 3700 10250 3700
Wire Wire Line
	10000 3800 10250 3800
Wire Wire Line
	10000 3900 10250 3900
Wire Wire Line
	10000 4000 10250 4000
$Comp
L power:GND #PWR0101
U 1 1 5BE4D09E
P 8150 2650
F 0 "#PWR0101" H 8150 2400 50  0001 C CNN
F 1 "GND" H 8155 2477 50  0000 C CNN
F 2 "" H 8150 2650 50  0001 C CNN
F 3 "" H 8150 2650 50  0001 C CNN
	1    8150 2650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5BE4D0CC
P 9600 2550
F 0 "#PWR0102" H 9600 2300 50  0001 C CNN
F 1 "GND" H 9605 2377 50  0000 C CNN
F 2 "" H 9600 2550 50  0001 C CNN
F 3 "" H 9600 2550 50  0001 C CNN
	1    9600 2550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0103
U 1 1 5BE4D0FE
P 9600 4300
F 0 "#PWR0103" H 9600 4050 50  0001 C CNN
F 1 "GND" H 9605 4127 50  0000 C CNN
F 2 "" H 9600 4300 50  0001 C CNN
F 3 "" H 9600 4300 50  0001 C CNN
	1    9600 4300
	1    0    0    -1  
$EndComp
Text Label 10050 1650 0    50   ~ 0
ROW_1
Text Label 10050 1750 0    50   ~ 0
ROW_2
Text Label 10050 1850 0    50   ~ 0
ROW_3
Text Label 10050 1950 0    50   ~ 0
ROW_4
Text Label 10050 2050 0    50   ~ 0
ROW_5
Text Label 10050 2150 0    50   ~ 0
ROW_6
Text Label 10050 2250 0    50   ~ 0
ROW_7
Text Label 10050 2350 0    50   ~ 0
ROW_8
Text Label 10050 3300 0    50   ~ 0
COL_1
Text Label 10050 3400 0    50   ~ 0
COL_2
Text Label 10050 3500 0    50   ~ 0
COL_3
Text Label 10050 3600 0    50   ~ 0
COL_4
Text Label 10050 3800 0    50   ~ 0
COL_6
Text Label 10050 3700 0    50   ~ 0
COL_5
Text Label 10050 3900 0    50   ~ 0
COL_7
Text Label 10050 4000 0    50   ~ 0
COL_8
Text Label 4450 2600 0    50   ~ 0
CAT_CLK
Wire Wire Line
	4900 2600 4450 2600
Text Label 7250 1950 0    50   ~ 0
CAT_CLK
Wire Wire Line
	4900 2800 4450 2800
Text Label 4450 2800 0    50   ~ 0
CAT_DATA
Text Label 7250 2250 0    50   ~ 0
CAT_DATA
Text Label 7250 2050 0    50   ~ 0
CAT_STROBE
Wire Wire Line
	9200 3400 8650 3400
Wire Wire Line
	9200 3500 8650 3500
Text Label 8650 3400 0    50   ~ 0
CAT_CLK
Text Label 8650 3500 0    50   ~ 0
CAT_STROBE
Text Label 6300 3800 0    50   ~ 0
CAT_STROBE
Wire Wire Line
	9200 3600 8650 3600
Wire Wire Line
	9200 3700 8800 3700
Wire Wire Line
	7750 1850 7550 1850
$Comp
L power:+3.3V #PWR0105
U 1 1 5BE58AE1
P 7550 1250
F 0 "#PWR0105" H 7550 1100 50  0001 C CNN
F 1 "+3.3V" H 7565 1423 50  0000 C CNN
F 2 "" H 7550 1250 50  0001 C CNN
F 3 "" H 7550 1250 50  0001 C CNN
	1    7550 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 3800 6750 3800
$Comp
L Device:R R1
U 1 1 5BE5A5A3
P 8800 3950
F 0 "R1" H 8870 3996 50  0000 L CNN
F 1 "1k" H 8870 3905 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 8730 3950 50  0001 C CNN
F 3 "~" H 8800 3950 50  0001 C CNN
	1    8800 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 3700 8800 3800
$Comp
L power:GND #PWR0106
U 1 1 5BE5BE28
P 8800 4300
F 0 "#PWR0106" H 8800 4050 50  0001 C CNN
F 1 "GND" H 8805 4127 50  0000 C CNN
F 2 "" H 8800 4300 50  0001 C CNN
F 3 "" H 8800 4300 50  0001 C CNN
	1    8800 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 2150 7250 2150
Wire Wire Line
	10000 1650 10300 1650
Wire Wire Line
	10000 1750 10300 1750
Wire Wire Line
	10000 1850 10300 1850
Wire Wire Line
	10000 1950 10300 1950
Wire Wire Line
	10000 2050 10300 2050
Wire Wire Line
	10000 2150 10300 2150
Wire Wire Line
	10000 2250 10300 2250
Wire Wire Line
	10000 2350 10300 2350
Wire Wire Line
	8800 4100 8800 4300
Wire Wire Line
	6200 3700 6750 3700
Wire Wire Line
	6200 3600 6750 3600
Wire Wire Line
	6200 2300 6750 2300
Wire Wire Line
	6200 2400 6750 2400
Wire Wire Line
	6200 2500 6750 2500
Wire Wire Line
	6200 2600 6750 2600
Text Label 6300 2300 0    50   ~ 0
UART4_TX
Text Label 6300 2400 0    50   ~ 0
UART4_RX
Text Label 6300 2500 0    50   ~ 0
UART2_TX
Text Label 6300 2600 0    50   ~ 0
UART2_RX
Text Label 6300 3700 0    50   ~ 0
SWCLK
Text Label 6300 3600 0    50   ~ 0
SWDIO
Wire Wire Line
	6200 3400 6750 3400
Wire Wire Line
	6200 3500 6750 3500
Text Label 6300 3400 0    50   ~ 0
USB_D-
Text Label 6300 3500 0    50   ~ 0
USB_D+
Wire Wire Line
	4900 2900 4450 2900
Wire Wire Line
	4900 3000 4450 3000
Text Label 4450 2900 0    50   ~ 0
UART1_TX
Text Label 4450 3000 0    50   ~ 0
UART1_RX
Wire Wire Line
	4900 3100 4450 3100
Wire Wire Line
	4900 3200 4450 3200
Wire Wire Line
	4900 3300 4450 3300
Text Label 4450 3300 0    50   ~ 0
UART3_TX
Wire Wire Line
	4900 3400 4450 3400
Text Label 4450 3400 0    50   ~ 0
UART3_RX
Wire Wire Line
	7250 1950 7750 1950
Wire Wire Line
	7250 2050 7750 2050
Wire Wire Line
	7250 2250 7750 2250
Text Label 4450 3100 0    50   ~ 0
I2C_SDA
Text Label 4450 3200 0    50   ~ 0
I2C_SCL
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 5BE8317F
P 1000 1750
F 0 "J1" H 920 2067 50  0000 C CNN
F 1 "Conn_01x04" H 920 1976 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 1000 1750 50  0001 C CNN
F 3 "~" H 1000 1750 50  0001 C CNN
	1    1000 1750
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 5BE833D2
P 1000 2450
F 0 "J2" H 920 2767 50  0000 C CNN
F 1 "Conn_01x04" H 920 2676 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 1000 2450 50  0001 C CNN
F 3 "~" H 1000 2450 50  0001 C CNN
	1    1000 2450
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J3
U 1 1 5BE83443
P 2700 1750
F 0 "J3" H 2780 1742 50  0000 L CNN
F 1 "Conn_01x04" H 2780 1651 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 2700 1750 50  0001 C CNN
F 3 "~" H 2700 1750 50  0001 C CNN
	1    2700 1750
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J4
U 1 1 5BE834A1
P 2700 2450
F 0 "J4" H 2780 2442 50  0000 L CNN
F 1 "Conn_01x04" H 2800 2350 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 2700 2450 50  0001 C CNN
F 3 "~" H 2700 2450 50  0001 C CNN
	1    2700 2450
	1    0    0    1   
$EndComp
Wire Wire Line
	1200 1650 1350 1650
Wire Wire Line
	1350 1650 1350 2550
Wire Wire Line
	1350 2550 1200 2550
Wire Wire Line
	1350 2550 1350 2700
Connection ~ 1350 2550
Wire Wire Line
	2500 1650 2350 1650
Wire Wire Line
	2350 1650 2350 2550
Wire Wire Line
	2350 2550 2500 2550
Wire Wire Line
	1200 2450 1400 2450
Wire Wire Line
	1400 2450 1400 1750
Wire Wire Line
	1200 1750 1400 1750
Connection ~ 1400 1750
Wire Wire Line
	1400 1750 1400 1350
Wire Wire Line
	2500 1750 2300 1750
Wire Wire Line
	2300 1750 2300 1350
Wire Wire Line
	2500 2450 2300 2450
Wire Wire Line
	2300 2450 2300 1750
Connection ~ 2300 1750
Wire Wire Line
	2350 2550 2350 2700
Connection ~ 2350 2550
$Comp
L power:GND #PWR0108
U 1 1 5BEAEEBF
P 2350 2700
F 0 "#PWR0108" H 2350 2450 50  0001 C CNN
F 1 "GND" H 2355 2527 50  0000 C CNN
F 2 "" H 2350 2700 50  0001 C CNN
F 3 "" H 2350 2700 50  0001 C CNN
	1    2350 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0109
U 1 1 5BEAEEFC
P 1350 2700
F 0 "#PWR0109" H 1350 2450 50  0001 C CNN
F 1 "GND" H 1355 2527 50  0000 C CNN
F 2 "" H 1350 2700 50  0001 C CNN
F 3 "" H 1350 2700 50  0001 C CNN
	1    1350 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 1850 1900 1850
Wire Wire Line
	2500 1950 1900 1950
Wire Wire Line
	2500 2250 1900 2250
Wire Wire Line
	2500 2350 1900 2350
Wire Wire Line
	5400 1000 5400 850 
Wire Wire Line
	5400 850  5500 850 
Wire Wire Line
	5800 850  5800 1000
Wire Wire Line
	5500 1000 5500 850 
Connection ~ 5500 850 
Wire Wire Line
	5500 850  5600 850 
Wire Wire Line
	5600 1000 5600 850 
Connection ~ 5600 850 
Wire Wire Line
	5600 850  5700 850 
Wire Wire Line
	5700 1000 5700 850 
Connection ~ 5700 850 
Wire Wire Line
	5700 850  5800 850 
Wire Wire Line
	5600 850  5600 800 
$Comp
L power:+3.3V #PWR0110
U 1 1 5BED9151
P 5600 800
F 0 "#PWR0110" H 5600 650 50  0001 C CNN
F 1 "+3.3V" H 5615 973 50  0000 C CNN
F 2 "" H 5600 800 50  0001 C CNN
F 3 "" H 5600 800 50  0001 C CNN
	1    5600 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 1850 1800 1850
Wire Wire Line
	1200 1950 1800 1950
Wire Wire Line
	1200 2250 1800 2250
Wire Wire Line
	1200 2350 1800 2350
Wire Wire Line
	7550 1250 7550 1850
Wire Wire Line
	8150 1450 8150 1250
$Comp
L power:+3.3V #PWR0111
U 1 1 5BE9B604
P 8150 1250
F 0 "#PWR0111" H 8150 1100 50  0001 C CNN
F 1 "+3.3V" H 8165 1423 50  0000 C CNN
F 2 "" H 8150 1250 50  0001 C CNN
F 3 "" H 8150 1250 50  0001 C CNN
	1    8150 1250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0112
U 1 1 5BE9B6E4
P 9600 1250
F 0 "#PWR0112" H 9600 1100 50  0001 C CNN
F 1 "+5V" H 9615 1423 50  0000 C CNN
F 2 "" H 9600 1250 50  0001 C CNN
F 3 "" H 9600 1250 50  0001 C CNN
	1    9600 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 1250 9600 1450
$Comp
L power:+3.3V #PWR0113
U 1 1 5BEA48D2
P 9600 3050
F 0 "#PWR0113" H 9600 2900 50  0001 C CNN
F 1 "+3.3V" H 9615 3223 50  0000 C CNN
F 2 "" H 9600 3050 50  0001 C CNN
F 3 "" H 9600 3050 50  0001 C CNN
	1    9600 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 3050 9600 3100
Wire Wire Line
	5400 4000 5400 4150
Wire Wire Line
	5400 4150 5500 4150
Wire Wire Line
	5700 4150 5700 4000
Wire Wire Line
	5600 4000 5600 4150
Connection ~ 5600 4150
Wire Wire Line
	5600 4150 5700 4150
Wire Wire Line
	5500 4000 5500 4150
Connection ~ 5500 4150
Wire Wire Line
	5500 4150 5600 4150
$Comp
L power:GND #PWR0114
U 1 1 5BEB6E28
P 5500 4250
F 0 "#PWR0114" H 5500 4000 50  0001 C CNN
F 1 "GND" H 5505 4077 50  0000 C CNN
F 2 "" H 5500 4250 50  0001 C CNN
F 3 "" H 5500 4250 50  0001 C CNN
	1    5500 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 4150 5500 4250
Text Label 4450 2700 0    50   ~ 0
nOE_COL
Text Label 7250 2150 0    50   ~ 0
nOE_ROW
Text Label 8650 3600 0    50   ~ 0
nOE_COL
Text Label 1450 5750 0    50   ~ 0
USB_D-
Text Label 1450 5850 0    50   ~ 0
USB_D+
$Comp
L power:GND #PWR0104
U 1 1 5BF13FE9
P 1050 6600
F 0 "#PWR0104" H 1050 6350 50  0001 C CNN
F 1 "GND" H 1055 6427 50  0000 C CNN
F 2 "" H 1050 6600 50  0001 C CNN
F 3 "" H 1050 6600 50  0001 C CNN
	1    1050 6600
	1    0    0    -1  
$EndComp
Text Label 1450 5650 0    50   ~ 0
USB_VBUS
$Comp
L Connector_Generic:Conn_01x04 J5
U 1 1 5BF3341E
P 1000 3700
F 0 "J5" H 920 4017 50  0000 C CNN
F 1 "Conn_01x04" H 920 3926 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 1000 3700 50  0001 C CNN
F 3 "~" H 1000 3700 50  0001 C CNN
	1    1000 3700
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J6
U 1 1 5BF33425
P 1000 4400
F 0 "J6" H 920 4717 50  0000 C CNN
F 1 "Conn_01x04" H 920 4626 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 1000 4400 50  0001 C CNN
F 3 "~" H 1000 4400 50  0001 C CNN
	1    1000 4400
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J7
U 1 1 5BF3342C
P 2700 3700
F 0 "J7" H 2780 3692 50  0000 L CNN
F 1 "Conn_01x04" H 2780 3601 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 2700 3700 50  0001 C CNN
F 3 "~" H 2700 3700 50  0001 C CNN
	1    2700 3700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J8
U 1 1 5BF33433
P 2700 4400
F 0 "J8" H 2780 4392 50  0000 L CNN
F 1 "Conn_01x04" H 2800 4300 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 2700 4400 50  0001 C CNN
F 3 "~" H 2700 4400 50  0001 C CNN
	1    2700 4400
	1    0    0    1   
$EndComp
Wire Wire Line
	1200 3600 1350 3600
Wire Wire Line
	1350 3600 1350 4500
Wire Wire Line
	1350 4500 1200 4500
Wire Wire Line
	1350 4500 1350 4650
Connection ~ 1350 4500
Wire Wire Line
	2500 3600 2350 3600
Wire Wire Line
	2350 3600 2350 4500
Wire Wire Line
	2350 4500 2500 4500
Wire Wire Line
	1200 4400 1400 4400
Wire Wire Line
	1400 4400 1400 3700
Wire Wire Line
	1200 3700 1400 3700
Connection ~ 1400 3700
Wire Wire Line
	1400 3700 1400 3300
Wire Wire Line
	2500 3700 2300 3700
Wire Wire Line
	2300 3700 2300 3300
Wire Wire Line
	2500 4400 2300 4400
Wire Wire Line
	2300 4400 2300 3700
Connection ~ 2300 3700
Wire Wire Line
	2350 4500 2350 4650
Connection ~ 2350 4500
$Comp
L power:GND #PWR0107
U 1 1 5BF3344E
P 2350 4650
F 0 "#PWR0107" H 2350 4400 50  0001 C CNN
F 1 "GND" H 2355 4477 50  0000 C CNN
F 2 "" H 2350 4650 50  0001 C CNN
F 3 "" H 2350 4650 50  0001 C CNN
	1    2350 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 5BF33454
P 1350 4650
F 0 "#PWR0115" H 1350 4400 50  0001 C CNN
F 1 "GND" H 1355 4477 50  0000 C CNN
F 2 "" H 1350 4650 50  0001 C CNN
F 3 "" H 1350 4650 50  0001 C CNN
	1    1350 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 3800 1900 3800
Wire Wire Line
	2500 3900 1900 3900
Wire Wire Line
	2500 4200 1900 4200
Wire Wire Line
	2500 4300 1900 4300
Wire Wire Line
	1200 3800 1800 3800
Wire Wire Line
	1200 3900 1800 3900
Wire Wire Line
	1200 4200 1800 4200
Wire Wire Line
	1200 4300 1800 4300
$Comp
L power:VCC #PWR0117
U 1 1 5BF09410
P 1400 1350
F 0 "#PWR0117" H 1400 1200 50  0001 C CNN
F 1 "VCC" H 1417 1523 50  0000 C CNN
F 2 "" H 1400 1350 50  0001 C CNN
F 3 "" H 1400 1350 50  0001 C CNN
	1    1400 1350
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0118
U 1 1 5BF09465
P 2300 1350
F 0 "#PWR0118" H 2300 1200 50  0001 C CNN
F 1 "VCC" H 2317 1523 50  0000 C CNN
F 2 "" H 2300 1350 50  0001 C CNN
F 3 "" H 2300 1350 50  0001 C CNN
	1    2300 1350
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0119
U 1 1 5BF094BA
P 1400 3300
F 0 "#PWR0119" H 1400 3150 50  0001 C CNN
F 1 "VCC" H 1417 3473 50  0000 C CNN
F 2 "" H 1400 3300 50  0001 C CNN
F 3 "" H 1400 3300 50  0001 C CNN
	1    1400 3300
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR0120
U 1 1 5BF0950F
P 2300 3300
F 0 "#PWR0120" H 2300 3150 50  0001 C CNN
F 1 "VCC" H 2317 3473 50  0000 C CNN
F 2 "" H 2300 3300 50  0001 C CNN
F 3 "" H 2300 3300 50  0001 C CNN
	1    2300 3300
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:MCP1703A-3302_SOT23 U5
U 1 1 5BF09A2F
P 4200 4950
F 0 "U5" H 4200 5192 50  0000 C CNN
F 1 "MCP1703A-3302_SOT23" H 4200 5101 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 4200 5150 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20005122B.pdf" H 4200 4900 50  0001 C CNN
	1    4200 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 4950 3650 4950
Text Label 3650 4950 0    50   ~ 0
VCC
Wire Wire Line
	4500 4950 4650 4950
$Comp
L power:GND #PWR0121
U 1 1 5BF1B1EE
P 4200 5300
F 0 "#PWR0121" H 4200 5050 50  0001 C CNN
F 1 "GND" H 4205 5127 50  0000 C CNN
F 2 "" H 4200 5300 50  0001 C CNN
F 3 "" H 4200 5300 50  0001 C CNN
	1    4200 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 5250 4200 5300
Text Label 1450 1850 0    50   ~ 0
UART1_TX
Text Label 1900 1850 0    50   ~ 0
UART2_RX
Text Label 1450 2350 0    50   ~ 0
UART1_RX
Text Label 1900 2350 0    50   ~ 0
UART2_TX
Text Label 1450 3800 0    50   ~ 0
UART3_TX
Text Label 1450 4300 0    50   ~ 0
UART3_RX
Text Label 1900 3800 0    50   ~ 0
UART4_RX
Text Label 1900 4300 0    50   ~ 0
UART4_TX
Text Label 4600 4950 0    50   ~ 0
+3V3
Wire Wire Line
	6200 3300 6750 3300
Text Label 6300 3300 0    50   ~ 0
nOE_ROW
Wire Wire Line
	4900 2700 4450 2700
Wire Wire Line
	8650 5450 8300 5450
Wire Wire Line
	8650 5550 8300 5550
Wire Wire Line
	8650 5650 8300 5650
Wire Wire Line
	8650 5750 8300 5750
Wire Wire Line
	8650 5850 8300 5850
Wire Wire Line
	8650 5950 8300 5950
Wire Wire Line
	8650 6050 8300 6050
Wire Wire Line
	8650 6150 8300 6150
Text Label 8300 5450 0    50   ~ 0
ROW_5
Text Label 8300 5550 0    50   ~ 0
ROW_7
Text Label 8300 5650 0    50   ~ 0
COL_2
Text Label 8300 5750 0    50   ~ 0
COL_3
Text Label 8300 5850 0    50   ~ 0
ROW_8
Text Label 8300 5950 0    50   ~ 0
COL_5
Text Label 8300 6050 0    50   ~ 0
ROW_6
Text Label 8300 6150 0    50   ~ 0
ROW_3
Text Label 9550 5450 0    50   ~ 0
COL_8
Text Label 9550 5550 0    50   ~ 0
COL_7
Text Label 9550 5650 0    50   ~ 0
ROW_2
Text Label 9550 5750 0    50   ~ 0
COL_1
Text Label 9550 5850 0    50   ~ 0
ROW_4
Text Label 9550 6050 0    50   ~ 0
COL_4
Text Label 9550 5950 0    50   ~ 0
COL_6
Text Label 9550 6150 0    50   ~ 0
ROW_1
Wire Wire Line
	9550 6050 9900 6050
$Comp
L TC15-11SRWA:TC15-11SRWA DS1
U 1 1 5C047E85
P 8650 5450
F 0 "DS1" H 9100 5715 50  0000 C CNN
F 1 "LDM-1588BS" H 9100 5624 50  0000 C CNN
F 2 "TC15-11SRWA:DIPS2794W50P254L3800H1065Q16N" H 9400 5550 50  0001 L CNN
F 3 "http://www.kingbrightusa.com/images/catalog/SPEC/TC15-11SRWA.pdf" H 9400 5450 50  0001 L CNN
F 4 "LED Displays & Accessories Dot Matrix Red 640nm Common Cathode" H 9400 5350 50  0001 L CNN "Description"
F 5 "10.65" H 9400 5250 50  0001 L CNN "Height"
F 6 "Kingbright" H 9400 5150 50  0001 L CNN "Manufacturer_Name"
F 7 "TC15-11SRWA" H 9400 5050 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "TC15-11SRWA" H 9400 4750 50  0001 L CNN "Arrow Part Number"
	1    8650 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 5450 9900 5450
Wire Wire Line
	9550 5550 9900 5550
Wire Wire Line
	9550 5650 9900 5650
Wire Wire Line
	9550 5750 9900 5750
Wire Wire Line
	9550 5850 9900 5850
Wire Wire Line
	9550 5950 9900 5950
Wire Wire Line
	9550 6150 9900 6150
Wire Wire Line
	4900 1200 4550 1200
Text Label 4550 1200 0    50   ~ 0
nRST
$Comp
L Connector_Generic:Conn_01x04 J10
U 1 1 5BEE1AF3
P 2150 5950
F 0 "J10" H 2070 5525 50  0000 C CNN
F 1 "Conn_01x04" H 2070 5616 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 2150 5950 50  0001 C CNN
F 3 "~" H 2150 5950 50  0001 C CNN
	1    2150 5950
	-1   0    0    1   
$EndComp
Wire Wire Line
	2350 6050 2450 6050
Wire Wire Line
	2450 6050 2450 6200
Wire Wire Line
	2350 5750 2650 5750
Wire Wire Line
	2350 5850 2650 5850
Wire Wire Line
	2350 5950 2650 5950
Text Label 2350 5750 0    50   ~ 0
nRST
$Comp
L power:GND #PWR0122
U 1 1 5BF0DA94
P 2450 6200
F 0 "#PWR0122" H 2450 5950 50  0001 C CNN
F 1 "GND" H 2455 6027 50  0000 C CNN
F 2 "" H 2450 6200 50  0001 C CNN
F 3 "" H 2450 6200 50  0001 C CNN
	1    2450 6200
	1    0    0    -1  
$EndComp
Text Label 2350 5850 0    50   ~ 0
SWDIO
Text Label 2350 5950 0    50   ~ 0
SWCLK
$Comp
L Device:C_Small C3
U 1 1 5BF2469C
P 3650 5100
F 0 "C3" H 3742 5146 50  0000 L CNN
F 1 "4.7u" H 3742 5055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3650 5100 50  0001 C CNN
F 3 "~" H 3650 5100 50  0001 C CNN
	1    3650 5100
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C4
U 1 1 5BF24740
P 4650 5100
F 0 "C4" H 4742 5146 50  0000 L CNN
F 1 "4.7u" H 4742 5055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4650 5100 50  0001 C CNN
F 3 "~" H 4650 5100 50  0001 C CNN
	1    4650 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 4950 3650 5000
Wire Wire Line
	3650 5200 3650 5300
Wire Wire Line
	3650 5300 4200 5300
Connection ~ 4200 5300
Wire Wire Line
	4650 4950 4650 5000
Connection ~ 4650 4950
Wire Wire Line
	4650 4950 4800 4950
Wire Wire Line
	4650 5200 4650 5300
Wire Wire Line
	4650 5300 4200 5300
NoConn ~ 1450 5950
NoConn ~ 10000 4100
Wire Wire Line
	3700 5900 4000 5900
Wire Wire Line
	4700 5900 4400 5900
Text Label 3700 5900 0    50   ~ 0
VCC
Text Label 4500 5900 0    50   ~ 0
+5V
Text Label 1450 1950 0    50   ~ 0
I2C_SDA
Text Label 1900 1950 0    50   ~ 0
I2C_SDA
Text Label 1450 2250 0    50   ~ 0
I2C_SCL
Text Label 1900 2250 0    50   ~ 0
I2C_SCL
Text Label 1450 3900 0    50   ~ 0
I2C_SDA
Text Label 1900 3900 0    50   ~ 0
I2C_SDA
Text Label 1450 4200 0    50   ~ 0
I2C_SCL
Text Label 1900 4200 0    50   ~ 0
I2C_SCL
$Comp
L Transistor_FET:IRLML6402 Q1
U 1 1 5BEBB389
P 4200 6000
F 0 "Q1" V 4543 6000 50  0000 C CNN
F 1 "IRLML6402" V 4452 6000 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 4400 5925 50  0001 L CIN
F 3 "https://www.infineon.com/dgdl/irlml6402pbf.pdf?fileId=5546d462533600a401535668d5c2263c" H 4200 6000 50  0001 L CNN
	1    4200 6000
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0123
U 1 1 5BEE0BDD
P 4200 6200
F 0 "#PWR0123" H 4200 5950 50  0001 C CNN
F 1 "GND" H 4205 6027 50  0000 C CNN
F 2 "" H 4200 6200 50  0001 C CNN
F 3 "" H 4200 6200 50  0001 C CNN
	1    4200 6200
	1    0    0    -1  
$EndComp
Text Label 4550 1600 0    50   ~ 0
LED
$Comp
L Device:LED D1
U 1 1 5BF12B88
P 7450 5550
F 0 "D1" V 7450 5350 50  0000 C CNN
F 1 "LED" V 7350 5350 50  0000 C CNN
F 2 "LED_SMD:LED_0805_2012Metric_Castellated" H 7450 5550 50  0001 C CNN
F 3 "~" H 7450 5550 50  0001 C CNN
	1    7450 5550
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R4
U 1 1 5BF12DFF
P 7450 5900
F 0 "R4" H 7600 5950 50  0000 C CNN
F 1 "680R" H 7650 5850 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 7380 5900 50  0001 C CNN
F 3 "~" H 7450 5900 50  0001 C CNN
	1    7450 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 5700 7450 5750
Wire Wire Line
	7450 6050 7450 6350
Wire Wire Line
	7450 5400 7450 5300
$Comp
L power:+3.3V #PWR0124
U 1 1 5BF45573
P 7450 5300
F 0 "#PWR0124" H 7450 5150 50  0001 C CNN
F 1 "+3.3V" H 7465 5473 50  0000 C CNN
F 2 "" H 7450 5300 50  0001 C CNN
F 3 "" H 7450 5300 50  0001 C CNN
	1    7450 5300
	1    0    0    -1  
$EndComp
Text Label 7450 6100 3    50   ~ 0
LED
$Comp
L dk_USB-DVI-HDMI-Connectors:10118194-0001LF J9
U 1 1 5BECC2C7
P 1150 5850
F 0 "J9" H 1213 6595 60  0000 C CNN
F 1 "10118194-0001LF" H 1213 6489 60  0000 C CNN
F 2 "digikey-footprints:USB_Micro_B_Female_10118194-0001LF" H 1350 6050 60  0001 L CNN
F 3 "https://cdn.amphenol-icc.com/media/wysiwyg/files/drawing/10118194.pdf" H 1350 6150 60  0001 L CNN
F 4 "609-4618-1-ND" H 1350 6250 60  0001 L CNN "Digi-Key_PN"
F 5 "10118194-0001LF" H 1350 6350 60  0001 L CNN "MPN"
F 6 "Connectors, Interconnects" H 1350 6450 60  0001 L CNN "Category"
F 7 "USB, DVI, HDMI Connectors" H 1350 6550 60  0001 L CNN "Family"
F 8 "https://cdn.amphenol-icc.com/media/wysiwyg/files/drawing/10118194.pdf" H 1350 6650 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/amphenol-fci/10118194-0001LF/609-4618-1-ND/2785382" H 1350 6750 60  0001 L CNN "DK_Detail_Page"
F 10 "CONN USB MICRO B RECPT SMT R/A" H 1350 6850 60  0001 L CNN "Description"
F 11 "Amphenol FCI" H 1350 6950 60  0001 L CNN "Manufacturer"
F 12 "Active" H 1350 7050 60  0001 L CNN "Status"
	1    1150 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 5650 1800 5650
Wire Wire Line
	1450 5750 1800 5750
Wire Wire Line
	1450 5850 1800 5850
Wire Wire Line
	1050 6450 1050 6600
$Comp
L power:GND #PWR0125
U 1 1 5BF467A5
P 1600 6150
F 0 "#PWR0125" H 1600 5900 50  0001 C CNN
F 1 "GND" H 1605 5977 50  0000 C CNN
F 2 "" H 1600 6150 50  0001 C CNN
F 3 "" H 1600 6150 50  0001 C CNN
	1    1600 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 6050 1600 6050
Wire Wire Line
	1600 6050 1600 6150
Wire Wire Line
	4900 1400 4550 1400
Text Label 4550 1400 0    50   ~ 0
BOOT0
$Comp
L Device:Jumper JP1
U 1 1 5C0B6011
P 4200 6800
F 0 "JP1" H 4200 7064 50  0001 C CNN
F 1 "Jumper" H 4200 6973 50  0001 C CNN
F 2 "Jumpers:SMT-JUMPER_2_NC_TRACE_NO-SILK" H 4200 6800 50  0001 C CNN
F 3 "~" H 4200 6800 50  0001 C CNN
	1    4200 6800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 6800 3550 6800
Wire Wire Line
	4500 6800 4850 6800
Text Label 3550 6800 0    50   ~ 0
USB_VBUS
Text Label 4550 6800 0    50   ~ 0
VCC
$Comp
L Device:C_Small C5
U 1 1 5BEDE6D7
P 5500 5100
F 0 "C5" H 5592 5146 50  0000 L CNN
F 1 "C_Small" H 5592 5055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5500 5100 50  0001 C CNN
F 3 "~" H 5500 5100 50  0001 C CNN
	1    5500 5100
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C6
U 1 1 5BEDE74B
P 5750 5100
F 0 "C6" H 5842 5146 50  0000 L CNN
F 1 "C_Small" H 5842 5055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5750 5100 50  0001 C CNN
F 3 "~" H 5750 5100 50  0001 C CNN
	1    5750 5100
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C7
U 1 1 5BEDE7AF
P 6000 5100
F 0 "C7" H 6092 5146 50  0000 L CNN
F 1 "C_Small" H 6092 5055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6000 5100 50  0001 C CNN
F 3 "~" H 6000 5100 50  0001 C CNN
	1    6000 5100
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C8
U 1 1 5BEDE813
P 6250 5100
F 0 "C8" H 6342 5146 50  0000 L CNN
F 1 "C_Small" H 6342 5055 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6250 5100 50  0001 C CNN
F 3 "~" H 6250 5100 50  0001 C CNN
	1    6250 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 5000 5500 4900
Wire Wire Line
	5500 4900 5750 4900
Wire Wire Line
	6250 4900 6250 5000
Wire Wire Line
	6000 5000 6000 4900
Connection ~ 6000 4900
Wire Wire Line
	6000 4900 6250 4900
Wire Wire Line
	5750 5000 5750 4900
Connection ~ 5750 4900
Wire Wire Line
	5750 4900 5900 4900
Wire Wire Line
	5500 5200 5500 5300
Wire Wire Line
	5500 5300 5750 5300
Wire Wire Line
	6250 5300 6250 5200
Wire Wire Line
	5750 5200 5750 5300
Connection ~ 5750 5300
Wire Wire Line
	5750 5300 5900 5300
Wire Wire Line
	6000 5200 6000 5300
Connection ~ 6000 5300
Wire Wire Line
	6000 5300 6250 5300
$Comp
L power:GND #PWR0126
U 1 1 5BF3032C
P 5900 5300
F 0 "#PWR0126" H 5900 5050 50  0001 C CNN
F 1 "GND" H 5905 5127 50  0000 C CNN
F 2 "" H 5900 5300 50  0001 C CNN
F 3 "" H 5900 5300 50  0001 C CNN
	1    5900 5300
	1    0    0    -1  
$EndComp
Connection ~ 5900 5300
Wire Wire Line
	5900 5300 6000 5300
$Comp
L power:+3.3V #PWR0127
U 1 1 5BF303B4
P 5900 4900
F 0 "#PWR0127" H 5900 4750 50  0001 C CNN
F 1 "+3.3V" H 5915 5073 50  0000 C CNN
F 2 "" H 5900 4900 50  0001 C CNN
F 3 "" H 5900 4900 50  0001 C CNN
	1    5900 4900
	1    0    0    -1  
$EndComp
Connection ~ 5900 4900
Wire Wire Line
	5900 4900 6000 4900
Wire Wire Line
	4900 1600 4550 1600
$Comp
L MCU_ST_STM32F0:STM32F072CBTx U1
U 1 1 5BEF07C3
P 5600 2500
F 0 "U1" H 5050 3950 50  0000 C CNN
F 1 "STM32F072CBTx" H 5550 3350 50  0000 C CNN
F 2 "Package_QFP:LQFP-48_7x7mm_P0.5mm" H 5000 1100 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00090510.pdf" H 5600 2500 50  0001 C CNN
	1    5600 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 7150 4450 7150
Text Label 4550 7150 0    50   ~ 0
BOOT0
$Comp
L Device:R_Small R2
U 1 1 5BF26B27
P 4450 7350
F 0 "R2" H 4509 7396 50  0000 L CNN
F 1 "1k" H 4509 7305 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4450 7350 50  0001 C CNN
F 3 "~" H 4450 7350 50  0001 C CNN
	1    4450 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 7250 4450 7150
Connection ~ 4450 7150
Wire Wire Line
	4450 7150 4850 7150
Wire Wire Line
	4450 7450 4450 7550
$Comp
L power:GND #PWR01
U 1 1 5BF40857
P 4450 7550
F 0 "#PWR01" H 4450 7300 50  0001 C CNN
F 1 "GND" H 4455 7377 50  0000 C CNN
F 2 "" H 4450 7550 50  0001 C CNN
F 3 "" H 4450 7550 50  0001 C CNN
	1    4450 7550
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open BOOT0
U 1 1 5BF40A5A
P 4100 7150
F 0 "BOOT0" H 4100 7300 50  0000 C CNN
F 1 "Jumper_2_Open" H 4100 7294 50  0001 C CNN
F 2 "Jumpers:SMT-JUMPER_2_NO_SILK" H 4100 7150 50  0001 C CNN
F 3 "~" H 4100 7150 50  0001 C CNN
	1    4100 7150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 7150 3550 7150
$Comp
L power:+3.3V #PWR0116
U 1 1 5BF4E4E1
P 3550 7150
F 0 "#PWR0116" H 3550 7000 50  0001 C CNN
F 1 "+3.3V" H 3565 7323 50  0000 C CNN
F 2 "" H 3550 7150 50  0001 C CNN
F 3 "" H 3550 7150 50  0001 C CNN
	1    3550 7150
	1    0    0    -1  
$EndComp
NoConn ~ 4900 1700
NoConn ~ 4900 1900
NoConn ~ 4900 2000
NoConn ~ 4900 2100
NoConn ~ 4900 2300
NoConn ~ 4900 2400
NoConn ~ 4900 2500
NoConn ~ 6200 2700
NoConn ~ 6200 2800
NoConn ~ 6200 2900
NoConn ~ 6200 3000
NoConn ~ 6200 3100
NoConn ~ 6200 3200
NoConn ~ 4900 3500
NoConn ~ 4900 3600
NoConn ~ 4900 3700
NoConn ~ 4900 3800
$EndSCHEMATC