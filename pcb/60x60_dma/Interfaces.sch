EESchema Schematic File Version 4
LIBS:bcl4v2-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 3950 2900 3    60   ~ 0
I2C_SCL
$Comp
L bcl4v2-rescue:VCC #PWR031
U 1 1 5BD6FE4D
P 3000 2000
F 0 "#PWR031" H 3000 1850 50  0001 C CNN
F 1 "VCC" H 3000 2150 50  0000 C CNN
F 2 "" H 3000 2000 50  0001 C CNN
F 3 "" H 3000 2000 50  0001 C CNN
	1    3000 2000
	-1   0    0    -1  
$EndComp
Text Label 3550 2950 3    60   ~ 0
I2C_SDA
Text Label 4050 2250 3    60   ~ 0
UART_RX4
Text Label 3450 2250 3    60   ~ 0
UART_TX4
$Comp
L bcl4v2-rescue:Conn_01x05 J1
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
L bcl4v2-rescue:Conn_01x05 J2
U 1 1 5BD6FE5D
P 4050 1900
F 0 "J2" H 4050 2200 50  0000 C CNN
F 1 "Conn_01x05" H 4050 1600 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 4050 1900 50  0001 C CNN
F 3 "" H 4050 1900 50  0001 C CNN
	1    4050 1900
	0    1    -1   0   
$EndComp
Text Label 3650 3000 3    60   ~ 0
IN1P
Text Label 3850 2350 3    60   ~ 0
BTN0
$Comp
L bcl4v2-rescue:GND #PWR032
U 1 1 5BD6FE74
P 4250 2250
F 0 "#PWR032" H 4250 2000 50  0001 C CNN
F 1 "GND" H 4250 2100 50  0000 C CNN
F 2 "" H 4250 2250 50  0001 C CNN
F 3 "" H 4250 2250 50  0001 C CNN
	1    4250 2250
	-1   0    0    -1  
$EndComp
Text Label 3550 4750 1    60   ~ 0
I2C_SCL
$Comp
L bcl4v2-rescue:VCC #PWR033
U 1 1 5BD7123B
P 3200 4950
F 0 "#PWR033" H 3200 4800 50  0001 C CNN
F 1 "VCC" H 3200 5100 50  0000 C CNN
F 2 "" H 3200 4950 50  0001 C CNN
F 3 "" H 3200 4950 50  0001 C CNN
	1    3200 4950
	1    0    0    -1  
$EndComp
Text Label 3950 4800 1    60   ~ 0
I2C_SDA
Text Label 4050 5000 1    60   ~ 0
UART_TX2
Text Label 3450 5000 1    60   ~ 0
UART_RX2
$Comp
L bcl4v2-rescue:Conn_01x05 J5
U 1 1 5BD71244
P 3450 5350
F 0 "J5" H 3450 5650 50  0000 C CNN
F 1 "Conn_01x05" H 3450 5050 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 3450 5350 50  0001 C CNN
F 3 "" H 3450 5350 50  0001 C CNN
	1    3450 5350
	0    -1   1    0   
$EndComp
$Comp
L bcl4v2-rescue:Conn_01x05 J4
U 1 1 5BD7124B
P 4050 5350
F 0 "J4" H 4050 5650 50  0000 C CNN
F 1 "Conn_01x05" H 4050 5050 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 4050 5350 50  0001 C CNN
F 3 "" H 4050 5350 50  0001 C CNN
	1    4050 5350
	0    1    1    0   
$EndComp
Text Label 3850 4350 1    60   ~ 0
IN0P
Text Label 3650 4900 1    60   ~ 0
BTN1
$Comp
L bcl4v2-rescue:GND #PWR034
U 1 1 5BD71262
P 4550 5250
F 0 "#PWR034" H 4550 5000 50  0001 C CNN
F 1 "GND" H 4550 5100 50  0000 C CNN
F 2 "" H 4550 5250 50  0001 C CNN
F 3 "" H 4550 5250 50  0001 C CNN
	1    4550 5250
	1    0    0    -1  
$EndComp
Text Label 5200 3300 2    60   ~ 0
UART_TX1
Text Label 5200 3900 2    60   ~ 0
UART_RX1
Text Label 5000 3800 2    60   ~ 0
I2C_SCL
$Comp
L bcl4v2-rescue:VCC #PWR035
U 1 1 5BD71F25
P 5300 4200
F 0 "#PWR035" H 5300 4050 50  0001 C CNN
F 1 "VCC" H 5300 4350 50  0000 C CNN
F 2 "" H 5300 4200 50  0001 C CNN
F 3 "" H 5300 4200 50  0001 C CNN
	1    5300 4200
	-1   0    0    1   
$EndComp
Text Label 5100 3400 2    60   ~ 0
I2C_SDA
Text Label 2350 3300 0    60   ~ 0
UART_RX3
Text Label 2400 3900 0    60   ~ 0
UART_TX3
Text Label 2350 3400 0    60   ~ 0
I2C_SCL
$Comp
L bcl4v2-rescue:VCC #PWR036
U 1 1 5BD71F2F
P 2300 4200
F 0 "#PWR036" H 2300 4050 50  0001 C CNN
F 1 "VCC" H 2300 4350 50  0000 C CNN
F 2 "" H 2300 4200 50  0001 C CNN
F 3 "" H 2300 4200 50  0001 C CNN
	1    2300 4200
	1    0    0    1   
$EndComp
Text Label 3000 3800 0    60   ~ 0
I2C_SDA
$Comp
L bcl4v2-rescue:Conn_01x05 J9
U 1 1 5BD71F36
P 5600 3900
F 0 "J9" H 5600 4200 50  0000 C CNN
F 1 "Conn_01x05" H 5600 3600 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 5600 3900 50  0001 C CNN
F 3 "" H 5600 3900 50  0001 C CNN
	1    5600 3900
	1    0    0    1   
$EndComp
$Comp
L bcl4v2-rescue:Conn_01x05 J8
U 1 1 5BD71F3D
P 5600 3300
F 0 "J8" H 5600 3600 50  0000 C CNN
F 1 "Conn_01x05" H 5600 3000 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 5600 3300 50  0001 C CNN
F 3 "" H 5600 3300 50  0001 C CNN
	1    5600 3300
	1    0    0    -1  
$EndComp
$Comp
L bcl4v2-rescue:Conn_01x05 J6
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
L bcl4v2-rescue:Conn_01x05 J7
U 1 1 5BD71F4B
P 2000 3300
F 0 "J7" H 2000 3600 50  0000 C CNN
F 1 "Conn_01x05" H 2000 3000 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 2000 3300 50  0001 C CNN
F 3 "" H 2000 3300 50  0001 C CNN
	1    2000 3300
	-1   0    0    -1  
$EndComp
Text Label 4500 3500 2    60   ~ 0
IN0P
Text Label 3000 3700 0    60   ~ 0
IN1P
Text Label 2450 3500 0    60   ~ 0
IN3
Text Label 5200 3700 2    60   ~ 0
IN2
$Comp
L bcl4v2-rescue:GND #PWR037
U 1 1 5BD71F73
P 5350 3050
F 0 "#PWR037" H 5350 2800 50  0001 C CNN
F 1 "GND" H 5350 2900 50  0000 C CNN
F 2 "" H 5350 3050 50  0001 C CNN
F 3 "" H 5350 3050 50  0001 C CNN
	1    5350 3050
	-1   0    0    1   
$EndComp
$Comp
L bcl4v2-rescue:GND #PWR038
U 1 1 5BD71F79
P 2250 3050
F 0 "#PWR038" H 2250 2800 50  0001 C CNN
F 1 "GND" H 2250 2900 50  0000 C CNN
F 2 "" H 2250 3050 50  0001 C CNN
F 3 "" H 2250 3050 50  0001 C CNN
	1    2250 3050
	1    0    0    1   
$EndComp
$Comp
L bcl4v2-rescue:Conn_01x01_Female J13
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
L bcl4v2-rescue:Conn_01x01_Female J12
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
L bcl4v2-rescue:Conn_01x01_Female J11
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
L bcl4v2-rescue:Conn_01x01_Female J10
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
L bcl4v2-rescue:USB_OTG J3
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
L bcl4v2-rescue:+5V #PWR039
U 1 1 5BD73B57
P 10000 1600
F 0 "#PWR039" H 10000 1450 50  0001 C CNN
F 1 "+5V" H 10000 1740 50  0000 C CNN
F 2 "" H 10000 1600 50  0001 C CNN
F 3 "" H 10000 1600 50  0001 C CNN
	1    10000 1600
	1    0    0    -1  
$EndComp
$Comp
L bcl4v2-rescue:Conn_01x02 J14
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
L bcl4v2-rescue:Conn_01x04 J15
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
L bcl4v2-rescue:+3.3V #PWR040
U 1 1 5BD73B6C
P 9600 3100
F 0 "#PWR040" H 9600 2950 50  0001 C CNN
F 1 "+3.3V" H 9600 3240 50  0000 C CNN
F 2 "" H 9600 3100 50  0001 C CNN
F 3 "" H 9600 3100 50  0001 C CNN
	1    9600 3100
	1    0    0    -1  
$EndComp
$Comp
L bcl4v2-rescue:GND #PWR041
U 1 1 5BD73B83
P 9500 4150
F 0 "#PWR041" H 9500 3900 50  0001 C CNN
F 1 "GND" H 9500 4000 50  0000 C CNN
F 2 "" H 9500 4150 50  0001 C CNN
F 3 "" H 9500 4150 50  0001 C CNN
	1    9500 4150
	1    0    0    -1  
$EndComp
$Comp
L bcl4v2-rescue:GND #PWR042
U 1 1 5BD73B89
P 9300 2350
F 0 "#PWR042" H 9300 2100 50  0001 C CNN
F 1 "GND" H 9300 2200 50  0000 C CNN
F 2 "" H 9300 2350 50  0001 C CNN
F 3 "" H 9300 2350 50  0001 C CNN
	1    9300 2350
	1    0    0    -1  
$EndComp
Text HLabel 1600 6100 0    60   Input ~ 0
UART_TX1
Text HLabel 1600 6700 0    60   Input ~ 0
UART_TX3
Text HLabel 1600 6850 0    60   Input ~ 0
UART_RX3
Text HLabel 1600 6250 0    60   Input ~ 0
UART_RX1
Text HLabel 1600 6400 0    60   Input ~ 0
UART_TX2
Text HLabel 1600 6550 0    60   Input ~ 0
UART_RX2
Text HLabel 1600 7000 0    60   Input ~ 0
UART_TX4
Text HLabel 1600 7150 0    60   Input ~ 0
UART_RX4
Text HLabel 5400 6350 0    60   Input ~ 0
I2C_SDA
Text HLabel 5400 6200 0    60   Input ~ 0
I2C_SCL
Text HLabel 10100 1950 2    60   Input ~ 0
USB_D-
Text HLabel 10100 1850 2    60   Input ~ 0
USB_D+
Text HLabel 3700 6150 0    60   Input ~ 0
BTN0
Text HLabel 3700 6300 0    60   Input ~ 0
BTN1
Text HLabel 3700 6450 0    60   Input ~ 0
IN0P
Text HLabel 3700 6600 0    60   Input ~ 0
IN1P
Text HLabel 3700 6750 0    60   Input ~ 0
IN2
Text HLabel 3700 6900 0    60   Input ~ 0
IN3
Text HLabel 9850 3800 2    60   Input ~ 0
NRST
Text HLabel 9850 3900 2    60   Input ~ 0
SWDIO
Text HLabel 9850 4000 2    60   Input ~ 0
SWCLK
Text HLabel 9850 3300 2    60   Input ~ 0
BOOT0
$Comp
L bcl4v2-rescue:+5V #PWR043
U 1 1 5BDD7179
P 6850 1850
F 0 "#PWR043" H 6850 1700 50  0001 C CNN
F 1 "+5V" H 6850 1990 50  0000 C CNN
F 2 "" H 6850 1850 50  0001 C CNN
F 3 "" H 6850 1850 50  0001 C CNN
	1    6850 1850
	1    0    0    -1  
$EndComp
$Comp
L bcl4v2-rescue:VCC #PWR044
U 1 1 5BDD717F
P 6350 1850
F 0 "#PWR044" H 6350 1700 50  0001 C CNN
F 1 "VCC" H 6350 2000 50  0000 C CNN
F 2 "" H 6350 1850 50  0001 C CNN
F 3 "" H 6350 1850 50  0001 C CNN
	1    6350 1850
	1    0    0    -1  
$EndComp
$Comp
L bcl4v2-rescue:Q_PMOS_GSD Q2
U 1 1 5BDD7457
P 6600 2050
F 0 "Q2" V 6550 2200 50  0000 L CNN
F 1 "IRLML6402" V 6500 1550 50  0000 L CNN
F 2 "kicad_libraries_new:badger_SOT-23_handsolder" H 6800 2150 50  0001 C CNN
F 3 "" H 6600 2050 50  0001 C CNN
	1    6600 2050
	0    -1   -1   0   
$EndComp
$Comp
L bcl4v2-rescue:GND #PWR045
U 1 1 5BDD7A19
P 6600 2350
F 0 "#PWR045" H 6600 2100 50  0001 C CNN
F 1 "GND" H 6600 2200 50  0000 C CNN
F 2 "" H 6600 2350 50  0001 C CNN
F 3 "" H 6600 2350 50  0001 C CNN
	1    6600 2350
	1    0    0    -1  
$EndComp
Text Notes 5400 1500 0    60   ~ 0
The P-MOSFET is protection against wrong polarity of input.\nBut only in case that no power is applied to the USB.\nThe P-MOSFET allows to powers the rail from the USB as well,\nbut once open by the USB power, there is no protection at all.
Text Label 1650 6850 0    60   ~ 0
UART_RX3
Text Label 1650 6700 0    60   ~ 0
UART_TX3
Text Label 1650 7000 0    60   ~ 0
UART_TX4
Text Label 1650 7150 0    60   ~ 0
UART_RX4
Text Label 1650 6100 0    60   ~ 0
UART_TX1
Text Label 1650 6250 0    60   ~ 0
UART_RX1
Text Label 1650 6550 0    60   ~ 0
UART_RX2
Text Label 1650 6400 0    60   ~ 0
UART_TX2
Text Label 3750 6300 0    60   ~ 0
BTN1
Text Label 3750 6450 0    60   ~ 0
IN0P
Text Label 3750 6600 0    60   ~ 0
IN1P
Text Label 3750 6900 0    60   ~ 0
IN3
Text Label 3750 6150 0    60   ~ 0
BTN0
Text Label 3750 6750 0    60   ~ 0
IN2
Text Label 5450 6200 0    60   ~ 0
I2C_SCL
Text Label 5450 6350 0    60   ~ 0
I2C_SDA
Wire Wire Line
	3850 2100 3850 2650
Wire Wire Line
	4050 2100 4050 2650
Wire Wire Line
	4250 2100 4250 2150
Wire Wire Line
	3250 2100 3250 2150
Connection ~ 4250 2150
Wire Wire Line
	4150 2200 4150 2100
Wire Wire Line
	3000 2200 3350 2200
Wire Wire Line
	3350 2100 3350 2200
Connection ~ 3350 2200
Wire Wire Line
	3450 2100 3450 2650
Wire Wire Line
	3650 3700 3650 2100
Wire Wire Line
	3950 2100 3950 3400
Wire Wire Line
	3550 2100 3550 3800
Wire Wire Line
	3850 3500 3850 5150
Wire Wire Line
	4050 5150 4050 4600
Wire Wire Line
	4250 5150 4250 5100
Wire Wire Line
	3250 5100 4250 5100
Wire Wire Line
	3250 5150 3250 5100
Connection ~ 4250 5100
Wire Wire Line
	4150 5050 4150 5150
Wire Wire Line
	3350 5150 3350 5050
Connection ~ 3350 5050
Wire Wire Line
	3450 5150 3450 4600
Wire Wire Line
	3650 5150 3650 4600
Wire Wire Line
	3850 3500 5400 3500
Wire Wire Line
	4650 3300 5400 3300
Wire Wire Line
	5400 3100 5350 3100
Wire Wire Line
	5350 4100 5350 3100
Wire Wire Line
	5400 4100 5350 4100
Connection ~ 5350 3100
Wire Wire Line
	5300 3200 5400 3200
Wire Wire Line
	5300 4200 5300 4000
Wire Wire Line
	5400 4000 5300 4000
Connection ~ 5300 4000
Wire Wire Line
	4700 3900 5400 3900
Wire Wire Line
	4600 3700 5400 3700
Wire Wire Line
	2200 3500 2750 3500
Wire Wire Line
	2200 3300 2750 3300
Wire Wire Line
	2200 3100 2250 3100
Wire Wire Line
	2250 4100 2250 3100
Wire Wire Line
	2200 4100 2250 4100
Connection ~ 2250 3100
Wire Wire Line
	2300 3200 2200 3200
Wire Wire Line
	2300 4200 2300 4000
Wire Wire Line
	2200 4000 2300 4000
Connection ~ 2300 4000
Wire Wire Line
	2200 3900 2750 3900
Wire Wire Line
	2200 3700 3650 3700
Wire Wire Line
	2200 3800 3550 3800
Wire Wire Line
	2200 3400 3750 3400
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
	9300 2250 9300 2300
Connection ~ 9300 2300
Wire Wire Line
	9600 1850 10100 1850
Wire Wire Line
	9600 1950 10100 1950
Wire Wire Line
	9600 1650 10000 1650
Wire Wire Line
	10000 1650 10000 1600
Wire Wire Line
	9500 3300 9850 3300
Wire Wire Line
	9500 3200 9600 3200
Wire Wire Line
	9600 3200 9600 3100
Wire Wire Line
	6600 2250 6600 2350
Wire Wire Line
	6850 1950 6800 1950
Wire Wire Line
	6350 1950 6400 1950
Wire Wire Line
	6850 1850 6850 1950
Wire Wire Line
	6350 1850 6350 1950
Wire Wire Line
	1600 6100 2100 6100
Wire Wire Line
	1600 6250 2100 6250
Wire Wire Line
	1600 6400 2100 6400
Wire Wire Line
	1600 6550 2100 6550
Wire Wire Line
	1600 6700 2100 6700
Wire Wire Line
	1600 6850 2100 6850
Wire Wire Line
	1600 7000 2100 7000
Wire Wire Line
	1600 7150 2100 7150
Wire Wire Line
	3700 6150 4000 6150
Wire Wire Line
	3700 6300 4000 6300
Wire Wire Line
	3700 6450 4000 6450
Wire Wire Line
	3700 6600 4000 6600
Wire Wire Line
	3700 6750 4000 6750
Wire Wire Line
	3700 6900 4000 6900
Wire Wire Line
	5400 6200 5850 6200
Wire Wire Line
	5400 6350 5850 6350
Connection ~ 3950 3400
Connection ~ 3550 3800
Wire Wire Line
	3000 2000 3000 2200
Wire Wire Line
	3250 2150 4250 2150
Wire Wire Line
	3200 4950 3200 5050
Wire Wire Line
	3200 5050 3350 5050
Wire Wire Line
	4550 5100 4550 5250
Wire Wire Line
	4250 2150 4250 2250
Wire Wire Line
	3350 2200 4150 2200
Wire Wire Line
	4250 5100 4550 5100
Wire Wire Line
	3350 5050 4150 5050
Wire Wire Line
	5350 3100 5350 3050
Wire Wire Line
	5300 4000 5300 3200
Wire Wire Line
	2250 3100 2250 3050
Wire Wire Line
	2300 4000 2300 3200
Wire Wire Line
	9300 2300 9300 2350
Wire Wire Line
	5400 3400 4200 3400
Wire Wire Line
	4200 3400 4200 3800
Wire Wire Line
	4200 3800 3950 3800
Wire Wire Line
	3950 3400 4150 3400
Wire Wire Line
	4150 3400 4150 3750
Wire Wire Line
	4150 3750 4350 3750
Wire Wire Line
	4350 3750 4350 3800
Wire Wire Line
	4350 3800 5400 3800
Wire Wire Line
	3950 3800 3950 5150
Connection ~ 3950 3800
Wire Wire Line
	3950 3800 3550 3800
Wire Wire Line
	3550 5150 3550 4050
Wire Wire Line
	3550 4050 3750 4050
Wire Wire Line
	3750 4050 3750 3400
Connection ~ 3750 3400
Wire Wire Line
	3750 3400 3950 3400
$EndSCHEMATC
