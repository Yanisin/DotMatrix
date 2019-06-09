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
Text Label 4350 2150 3    60   ~ 0
I2C_SCL
$Comp
L bcl4v2-rescue:VCC #PWR031
U 1 1 5BD6FE4D
P 3400 1850
F 0 "#PWR031" H 3400 1700 50  0001 C CNN
F 1 "VCC" H 3400 2000 50  0000 C CNN
F 2 "" H 3400 1850 50  0001 C CNN
F 3 "" H 3400 1850 50  0001 C CNN
	1    3400 1850
	-1   0    0    -1  
$EndComp
Text Label 3950 2150 3    60   ~ 0
I2C_SDA
Text Label 4450 2100 3    60   ~ 0
UART_RX4
Text Label 3850 2100 3    60   ~ 0
UART_TX4
$Comp
L bcl4v2-rescue:Conn_01x05 J1
U 1 1 5BD6FE56
P 3850 1750
F 0 "J1" H 3850 2050 50  0000 C CNN
F 1 "Conn_01x05" H 3850 1450 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 3850 1750 50  0001 C CNN
F 3 "" H 3850 1750 50  0001 C CNN
	1    3850 1750
	0    -1   -1   0   
$EndComp
$Comp
L bcl4v2-rescue:Conn_01x05 J2
U 1 1 5BD6FE5D
P 4450 1750
F 0 "J2" H 4450 2050 50  0000 C CNN
F 1 "Conn_01x05" H 4450 1450 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 4450 1750 50  0001 C CNN
F 3 "" H 4450 1750 50  0001 C CNN
	1    4450 1750
	0    1    -1   0   
$EndComp
Text Label 4050 2250 3    60   ~ 0
IN1P
Text Label 4250 2200 3    60   ~ 0
BTN0
$Comp
L bcl4v2-rescue:GND #PWR032
U 1 1 5BD6FE74
P 4650 2100
F 0 "#PWR032" H 4650 1850 50  0001 C CNN
F 1 "GND" H 4650 1950 50  0000 C CNN
F 2 "" H 4650 2100 50  0001 C CNN
F 3 "" H 4650 2100 50  0001 C CNN
	1    4650 2100
	-1   0    0    -1  
$EndComp
Text Label 3950 4250 1    60   ~ 0
I2C_SCL
$Comp
L bcl4v2-rescue:VCC #PWR033
U 1 1 5BD7123B
P 3600 4200
F 0 "#PWR033" H 3600 4050 50  0001 C CNN
F 1 "VCC" H 3600 4350 50  0000 C CNN
F 2 "" H 3600 4200 50  0001 C CNN
F 3 "" H 3600 4200 50  0001 C CNN
	1    3600 4200
	1    0    0    -1  
$EndComp
Text Label 4350 4250 1    60   ~ 0
I2C_SDA
Text Label 4450 4250 1    60   ~ 0
UART_TX2
Text Label 3850 4250 1    60   ~ 0
UART_RX2
$Comp
L bcl4v2-rescue:Conn_01x05 J5
U 1 1 5BD71244
P 3850 4600
F 0 "J5" H 3850 4900 50  0000 C CNN
F 1 "Conn_01x05" H 3850 4300 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 3850 4600 50  0001 C CNN
F 3 "" H 3850 4600 50  0001 C CNN
	1    3850 4600
	0    -1   1    0   
$EndComp
$Comp
L bcl4v2-rescue:Conn_01x05 J4
U 1 1 5BD7124B
P 4450 4600
F 0 "J4" H 4450 4900 50  0000 C CNN
F 1 "Conn_01x05" H 4450 4300 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 4450 4600 50  0001 C CNN
F 3 "" H 4450 4600 50  0001 C CNN
	1    4450 4600
	0    1    1    0   
$EndComp
Text Label 4250 4250 1    60   ~ 0
IN0P
Text Label 4050 4250 1    60   ~ 0
BTN1
$Comp
L bcl4v2-rescue:GND #PWR034
U 1 1 5BD71262
P 4950 4500
F 0 "#PWR034" H 4950 4250 50  0001 C CNN
F 1 "GND" H 4950 4350 50  0000 C CNN
F 2 "" H 4950 4500 50  0001 C CNN
F 3 "" H 4950 4500 50  0001 C CNN
	1    4950 4500
	1    0    0    -1  
$EndComp
Text Label 5250 2850 2    60   ~ 0
UART_TX1
Text Label 5250 3450 2    60   ~ 0
UART_RX1
Text Label 5200 3350 2    60   ~ 0
I2C_SCL
$Comp
L bcl4v2-rescue:VCC #PWR035
U 1 1 5BD71F25
P 5300 3750
F 0 "#PWR035" H 5300 3600 50  0001 C CNN
F 1 "VCC" H 5300 3900 50  0000 C CNN
F 2 "" H 5300 3750 50  0001 C CNN
F 3 "" H 5300 3750 50  0001 C CNN
	1    5300 3750
	-1   0    0    1   
$EndComp
Text Label 5200 2950 2    60   ~ 0
I2C_SDA
Text Label 3100 2850 0    60   ~ 0
UART_RX3
Text Label 3150 3450 0    60   ~ 0
UART_TX3
Text Label 3100 2950 0    60   ~ 0
I2C_SCL
$Comp
L bcl4v2-rescue:VCC #PWR036
U 1 1 5BD71F2F
P 3050 3750
F 0 "#PWR036" H 3050 3600 50  0001 C CNN
F 1 "VCC" H 3050 3900 50  0000 C CNN
F 2 "" H 3050 3750 50  0001 C CNN
F 3 "" H 3050 3750 50  0001 C CNN
	1    3050 3750
	1    0    0    1   
$EndComp
Text Label 3150 3350 0    60   ~ 0
I2C_SDA
$Comp
L bcl4v2-rescue:Conn_01x05 J9
U 1 1 5BD71F36
P 5600 3450
F 0 "J9" H 5600 3750 50  0000 C CNN
F 1 "Conn_01x05" H 5600 3150 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 5600 3450 50  0001 C CNN
F 3 "" H 5600 3450 50  0001 C CNN
	1    5600 3450
	1    0    0    1   
$EndComp
$Comp
L bcl4v2-rescue:Conn_01x05 J8
U 1 1 5BD71F3D
P 5600 2850
F 0 "J8" H 5600 3150 50  0000 C CNN
F 1 "Conn_01x05" H 5600 2550 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 5600 2850 50  0001 C CNN
F 3 "" H 5600 2850 50  0001 C CNN
	1    5600 2850
	1    0    0    -1  
$EndComp
$Comp
L bcl4v2-rescue:Conn_01x05 J6
U 1 1 5BD71F44
P 2750 3450
F 0 "J6" H 2750 3750 50  0000 C CNN
F 1 "Conn_01x05" H 2750 3150 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 2750 3450 50  0001 C CNN
F 3 "" H 2750 3450 50  0001 C CNN
	1    2750 3450
	-1   0    0    1   
$EndComp
$Comp
L bcl4v2-rescue:Conn_01x05 J7
U 1 1 5BD71F4B
P 2750 2850
F 0 "J7" H 2750 3150 50  0000 C CNN
F 1 "Conn_01x05" H 2750 2550 50  0000 C CNN
F 2 "kicad_libraries_new:badger_Pin_Hdr_1x05_2.54mm_handsolder" H 2750 2850 50  0001 C CNN
F 3 "" H 2750 2850 50  0001 C CNN
	1    2750 2850
	-1   0    0    -1  
$EndComp
Text Label 5050 3050 2    60   ~ 0
IN0P
Text Label 3150 3250 0    60   ~ 0
IN1P
Text Label 3200 3050 0    60   ~ 0
IN3
Text Label 5000 3250 2    60   ~ 0
IN2
$Comp
L bcl4v2-rescue:GND #PWR037
U 1 1 5BD71F73
P 5350 2600
F 0 "#PWR037" H 5350 2350 50  0001 C CNN
F 1 "GND" H 5350 2450 50  0000 C CNN
F 2 "" H 5350 2600 50  0001 C CNN
F 3 "" H 5350 2600 50  0001 C CNN
	1    5350 2600
	-1   0    0    1   
$EndComp
$Comp
L bcl4v2-rescue:GND #PWR038
U 1 1 5BD71F79
P 3000 2600
F 0 "#PWR038" H 3000 2350 50  0001 C CNN
F 1 "GND" H 3000 2450 50  0000 C CNN
F 2 "" H 3000 2600 50  0001 C CNN
F 3 "" H 3000 2600 50  0001 C CNN
	1    3000 2600
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
P 9300 1900
F 0 "J3" H 9100 2350 50  0000 L CNN
F 1 "USB_OTG" H 9100 2250 50  0000 L CNN
F 2 "Connect:USB_Micro-B" H 9450 1850 50  0001 C CNN
F 3 "" H 9450 1850 50  0001 C CNN
	1    9300 1900
	1    0    0    -1  
$EndComp
NoConn ~ 9600 2100
$Comp
L bcl4v2-rescue:+5V #PWR039
U 1 1 5BD73B57
P 9650 1650
F 0 "#PWR039" H 9650 1500 50  0001 C CNN
F 1 "+5V" H 9650 1790 50  0000 C CNN
F 2 "" H 9650 1650 50  0001 C CNN
F 3 "" H 9650 1650 50  0001 C CNN
	1    9650 1650
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
P 9300 2400
F 0 "#PWR042" H 9300 2150 50  0001 C CNN
F 1 "GND" H 9300 2250 50  0000 C CNN
F 2 "" H 9300 2400 50  0001 C CNN
F 3 "" H 9300 2400 50  0001 C CNN
	1    9300 2400
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
Text HLabel 9800 2000 2    60   Input ~ 0
USB_D-
Text HLabel 9800 1900 2    60   Input ~ 0
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
	4250 1950 4250 2500
Wire Wire Line
	4450 1950 4450 2500
Wire Wire Line
	4650 1950 4650 2000
Wire Wire Line
	3650 1950 3650 2000
Connection ~ 4650 2000
Wire Wire Line
	4550 2050 4550 1950
Wire Wire Line
	3400 2050 3750 2050
Wire Wire Line
	3750 1950 3750 2050
Connection ~ 3750 2050
Wire Wire Line
	3850 1950 3850 2500
Wire Wire Line
	4450 4400 4450 3850
Wire Wire Line
	4650 4400 4650 4350
Wire Wire Line
	3650 4350 4550 4350
Wire Wire Line
	3650 4400 3650 4350
Connection ~ 4650 4350
Wire Wire Line
	4550 4300 4550 4350
Wire Wire Line
	3750 4400 3750 4300
Connection ~ 3750 4300
Wire Wire Line
	3850 4400 3850 3850
Wire Wire Line
	4050 4400 4050 3850
Wire Wire Line
	5400 2650 5350 2650
Wire Wire Line
	5400 3650 5350 3650
Wire Wire Line
	5300 2750 5400 2750
Wire Wire Line
	5300 3750 5300 3550
Wire Wire Line
	5400 3550 5300 3550
Wire Wire Line
	2950 3050 3500 3050
Wire Wire Line
	2950 2850 3500 2850
Wire Wire Line
	2950 2650 3000 2650
Wire Wire Line
	3000 3650 3000 2650
Wire Wire Line
	2950 3650 3000 3650
Connection ~ 3000 2650
Wire Wire Line
	3050 2750 2950 2750
Wire Wire Line
	3050 3750 3050 3550
Wire Wire Line
	2950 3550 3050 3550
Connection ~ 3050 3550
Wire Wire Line
	2950 3450 3500 3450
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
	9200 2300 9200 2350
Wire Wire Line
	9200 2350 9300 2350
Wire Wire Line
	9300 2300 9300 2350
Connection ~ 9300 2350
Wire Wire Line
	9500 3300 9850 3300
Wire Wire Line
	9500 3200 9600 3200
Wire Wire Line
	9600 3200 9600 3100
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
Wire Wire Line
	3400 1850 3400 2050
Wire Wire Line
	3650 2000 4650 2000
Wire Wire Line
	3600 4200 3600 4300
Wire Wire Line
	3600 4300 3750 4300
Wire Wire Line
	4850 4350 4850 4500
Wire Wire Line
	4650 2000 4650 2100
Wire Wire Line
	3750 2050 4550 2050
Wire Wire Line
	4550 4350 4650 4350
Wire Wire Line
	3750 4300 4550 4300
Wire Wire Line
	5350 2650 5350 2600
Wire Wire Line
	3000 2650 3000 2600
Wire Wire Line
	3050 3550 3050 2750
Wire Wire Line
	9300 2350 9300 2400
$Comp
L Connector:TestPoint TP4
U 1 1 5C7F78D4
P 8000 5300
F 0 "TP4" H 8058 5420 50  0000 L CNN
F 1 "TestPoint" H 8058 5329 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 8200 5300 50  0001 C CNN
F 3 "~" H 8200 5300 50  0001 C CNN
	1    8000 5300
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint TP5
U 1 1 5C7F782D
P 8100 5500
F 0 "TP5" H 8158 5620 50  0000 L CNN
F 1 "TestPoint" H 8158 5529 50  0000 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 8300 5500 50  0001 C CNN
F 3 "~" H 8300 5500 50  0001 C CNN
	1    8100 5500
	1    0    0    -1  
$EndComp
Text Label 9650 1900 0    60   ~ 0
D+
Text Label 9650 2000 0    60   ~ 0
D-
Text Label 8150 5650 0    60   ~ 0
D+
Text Label 8150 5750 0    60   ~ 0
D-
Wire Wire Line
	8100 5500 8100 5650
Wire Wire Line
	8100 5650 8300 5650
Wire Wire Line
	8000 5300 8000 5750
Wire Wire Line
	8000 5750 8300 5750
Wire Wire Line
	9650 1650 9650 1700
Wire Wire Line
	9650 1700 9600 1700
Wire Wire Line
	9600 2000 9800 2000
Wire Wire Line
	9600 1900 9800 1900
Wire Wire Line
	5400 2850 4750 2850
Wire Wire Line
	4750 3250 5400 3250
Connection ~ 5350 2650
Wire Wire Line
	5350 3650 5350 2650
Connection ~ 5300 3550
Wire Wire Line
	5300 2750 5300 3550
Wire Wire Line
	4750 3450 5400 3450
Wire Wire Line
	3950 2950 4350 2950
Wire Wire Line
	3950 1950 3950 2950
Wire Wire Line
	3950 3350 3950 2950
Wire Wire Line
	2950 3350 3950 3350
Connection ~ 3950 2950
Wire Wire Line
	4350 3350 3950 3350
Wire Wire Line
	4350 3350 4350 4400
Connection ~ 3950 3350
Wire Wire Line
	4350 3350 4350 2950
Connection ~ 4350 3350
Wire Wire Line
	3950 3400 4000 3400
Wire Wire Line
	4000 3400 4000 3300
Wire Wire Line
	3950 3400 3950 4400
Wire Wire Line
	4400 3350 4400 3300
Wire Wire Line
	4000 3300 4300 3300
Wire Wire Line
	4300 3300 4300 3000
Wire Wire Line
	4300 3000 4000 3000
Connection ~ 4300 3300
Wire Wire Line
	4300 3300 4400 3300
Wire Wire Line
	4000 3000 4000 3300
Connection ~ 4000 3300
Wire Wire Line
	4000 3000 3900 3000
Wire Wire Line
	3900 3000 3900 2950
Wire Wire Line
	2950 2950 3900 2950
Connection ~ 4000 3000
Wire Wire Line
	4300 3000 4300 2900
Wire Wire Line
	4300 2900 4350 2900
Wire Wire Line
	4350 1950 4350 2900
Connection ~ 4300 3000
Wire Wire Line
	4250 3050 4250 4400
Wire Wire Line
	4050 1950 4050 3250
Wire Wire Line
	2950 3250 4050 3250
Connection ~ 4550 4350
Wire Wire Line
	4550 4350 4550 4400
Wire Wire Line
	4650 4350 4850 4350
Wire Wire Line
	5400 3050 4250 3050
Wire Wire Line
	5400 2950 4350 2950
Connection ~ 4350 2950
Wire Wire Line
	4400 3350 5400 3350
$EndSCHEMATC
