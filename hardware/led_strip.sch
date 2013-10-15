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
LIBS:special
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
LIBS:tinkerforge
LIBS:led_strip-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "LED Strip Bricklet"
Date "20 sep 2013"
Rev "1.0"
Comp "Tinkerforge GmbH"
Comment1 "Licensed under CERN OHL v.1.1"
Comment2 "Copyright (©) 2013, B.Nordmeyer <bastian@tinkerforge.com>"
Comment3 ""
Comment4 ""
$EndDescr
NoConn ~ 4100 3650
$Comp
L GND #PWR01
U 1 1 509A86EF
P 4500 3250
F 0 "#PWR01" H 4500 3250 30  0001 C CNN
F 1 "GND" H 4500 3180 30  0001 C CNN
F 2 "" H 4500 3250 60  0001 C CNN
F 3 "" H 4500 3250 60  0001 C CNN
	1    4500 3250
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR02
U 1 1 509A86E5
P 4100 3100
F 0 "#PWR02" H 4100 3200 30  0001 C CNN
F 1 "VCC" H 4100 3200 30  0000 C CNN
F 2 "" H 4100 3100 60  0001 C CNN
F 3 "" H 4100 3100 60  0001 C CNN
	1    4100 3100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 509A86CD
P 4900 3900
F 0 "#PWR03" H 4900 3900 30  0001 C CNN
F 1 "GND" H 4900 3830 30  0001 C CNN
F 2 "" H 4900 3900 60  0001 C CNN
F 3 "" H 4900 3900 60  0001 C CNN
	1    4900 3900
	1    0    0    -1  
$EndComp
Text Notes 550  7200 0    40   ~ 0
Copyright Tinkerforge GmbH 2013.\nThis documentation describes Open Hardware and is licensed under the\nCERN OHL v. 1.1.\nYou may redistribute and modify this documentation under the terms of the\nCERN OHL v.1.1. (http://ohwr.org/cernohl). This documentation is distributed\nWITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING OF\nMERCHANTABILITY, SATISFACTORY QUALITY AND FITNESS FOR A\nPARTICULAR PURPOSE. Please see the CERN OHL v.1.1 for applicable\nconditions\n
$Comp
L DRILL U2
U 1 1 4C6050A5
P 10700 6500
F 0 "U2" H 10750 6550 60  0001 C CNN
F 1 "DRILL" H 10700 6500 60  0000 C CNN
F 2 "DRILL_NP" H 10700 6500 60  0001 C CNN
F 3 "" H 10700 6500 60  0001 C CNN
	1    10700 6500
	1    0    0    -1  
$EndComp
$Comp
L DRILL U3
U 1 1 4C6050A2
P 10700 6700
F 0 "U3" H 10750 6750 60  0001 C CNN
F 1 "DRILL" H 10700 6700 60  0000 C CNN
F 2 "DRILL_NP" H 10700 6700 60  0001 C CNN
F 3 "" H 10700 6700 60  0001 C CNN
	1    10700 6700
	1    0    0    -1  
$EndComp
$Comp
L DRILL U5
U 1 1 4C60509F
P 11050 6700
F 0 "U5" H 11100 6750 60  0001 C CNN
F 1 "DRILL" H 11050 6700 60  0000 C CNN
F 2 "DRILL_NP" H 11050 6700 60  0001 C CNN
F 3 "" H 11050 6700 60  0001 C CNN
	1    11050 6700
	1    0    0    -1  
$EndComp
$Comp
L DRILL U4
U 1 1 4C605099
P 11050 6500
F 0 "U4" H 11100 6550 60  0001 C CNN
F 1 "DRILL" H 11050 6500 60  0000 C CNN
F 2 "DRILL_NP" H 11050 6500 60  0001 C CNN
F 3 "" H 11050 6500 60  0001 C CNN
	1    11050 6500
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 4C5FD6ED
P 4300 3200
F 0 "C1" V 4450 3200 50  0000 L CNN
F 1 "100nF" V 4150 3100 50  0000 L CNN
F 2 "0603" H 4300 3200 60  0001 C CNN
F 3 "" H 4300 3200 60  0001 C CNN
	1    4300 3200
	0    -1   -1   0   
$EndComp
$Comp
L CAT24C U1
U 1 1 4C5FD337
P 4500 3950
F 0 "U1" H 4350 4450 60  0000 C CNN
F 1 "M24C64" H 4500 3950 60  0000 C CNN
F 2 "SOIC8" H 4500 3950 60  0001 C CNN
F 3 "" H 4500 3950 60  0001 C CNN
	1    4500 3950
	-1   0    0    -1  
$EndComp
$Comp
L VCC #PWR04
U 1 1 4C5FCFB4
P 3550 3350
F 0 "#PWR04" H 3550 3450 30  0001 C CNN
F 1 "VCC" H 3550 3450 30  0000 C CNN
F 2 "" H 3550 3350 60  0001 C CNN
F 3 "" H 3550 3350 60  0001 C CNN
	1    3550 3350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 4C5FCF5E
P 3650 3700
F 0 "#PWR05" H 3650 3700 30  0001 C CNN
F 1 "GND" H 3650 3630 30  0001 C CNN
F 2 "" H 3650 3700 60  0001 C CNN
F 3 "" H 3650 3700 60  0001 C CNN
	1    3650 3700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 4C5FCF4F
P 2950 4600
F 0 "#PWR06" H 2950 4600 30  0001 C CNN
F 1 "GND" H 2950 4530 30  0001 C CNN
F 2 "" H 2950 4600 60  0001 C CNN
F 3 "" H 2950 4600 60  0001 C CNN
	1    2950 4600
	1    0    0    -1  
$EndComp
$Comp
L CON-SENSOR P1
U 1 1 4C5FCF27
P 2950 3900
F 0 "P1" H 2700 4450 60  0000 C CNN
F 1 "CON-SENSOR" V 3100 3900 60  0000 C CNN
F 2 "CON-SENSOR" H 2950 3900 60  0001 C CNN
F 3 "" H 2950 3900 60  0001 C CNN
	1    2950 3900
	-1   0    0    -1  
$EndComp
$Comp
L R_PACK4 RP1
U 1 1 5200B149
P 5350 4850
F 0 "RP1" H 5350 5300 40  0000 C CNN
F 1 "4k7" H 5350 4800 40  0000 C CNN
F 2 "4X0603" H 5350 4850 60  0001 C CNN
F 3 "" H 5350 4850 60  0000 C CNN
	1    5350 4850
	0    -1   -1   0   
$EndComp
$Comp
L R R2
U 1 1 5200B54D
P 4800 4050
F 0 "R2" V 4880 4050 50  0000 C CNN
F 1 "10k" V 4800 4050 50  0000 C CNN
F 2 "0603" H 4800 4050 60  0001 C CNN
F 3 "" H 4800 4050 60  0000 C CNN
	1    4800 4050
	0    -1   -1   0   
$EndComp
$Comp
L R R1
U 1 1 5200B597
P 4550 4750
F 0 "R1" V 4630 4750 50  0000 C CNN
F 1 "1k" V 4550 4750 50  0000 C CNN
F 2 "0603" H 4550 4750 60  0001 C CNN
F 3 "" H 4550 4750 60  0000 C CNN
	1    4550 4750
	-1   0    0    1   
$EndComp
Wire Wire Line
	4500 3200 4500 3250
Wire Wire Line
	5000 3850 4900 3850
Wire Wire Line
	5000 3850 5000 3650
Wire Wire Line
	5000 3650 4900 3650
Wire Wire Line
	4100 3850 3400 3850
Wire Wire Line
	3550 3350 3550 3650
Wire Wire Line
	3550 3650 3400 3650
Wire Wire Line
	2950 4600 2950 4500
Wire Wire Line
	3400 3550 3650 3550
Wire Wire Line
	3650 3550 3650 3700
Wire Wire Line
	4100 3750 3400 3750
Wire Wire Line
	4900 3850 4900 3900
Wire Wire Line
	4900 3650 4900 3550
Connection ~ 4900 3650
Connection ~ 4900 3850
Wire Wire Line
	4100 3550 4100 3100
Connection ~ 4100 3200
Wire Wire Line
	3400 3950 5050 3950
Wire Wire Line
	5050 3950 5050 3750
Wire Wire Line
	5050 3750 4900 3750
Wire Wire Line
	7450 4450 7550 4450
Wire Wire Line
	7450 3950 7450 4450
Wire Wire Line
	7250 4250 8050 4250
Wire Wire Line
	8050 4250 8050 4450
Wire Wire Line
	8050 4450 8100 4450
Wire Wire Line
	8100 4350 8100 4200
Wire Wire Line
	8100 4200 7550 4200
Wire Wire Line
	7550 3950 7550 4350
Wire Wire Line
	6700 4550 7550 4550
Wire Wire Line
	7450 4550 7450 4750
Wire Wire Line
	7450 4750 8050 4750
Wire Wire Line
	8050 4750 8050 4550
Wire Wire Line
	8050 4550 8100 4550
Wire Wire Line
	7550 4650 7550 4800
Wire Wire Line
	7550 4800 8100 4800
Wire Wire Line
	8100 4800 8100 4650
$Comp
L AKL_5_2 J1
U 1 1 5200B695
P 7500 3250
F 0 "J1" H 7600 3245 60  0000 C CNN
F 1 "AKL_5_2" H 7485 3150 60  0000 C CNN
F 2 "AKL_5_2" H 7500 3250 60  0001 C CNN
F 3 "~" H 7500 3250 60  0000 C CNN
	1    7500 3250
	0    -1   -1   0   
$EndComp
Connection ~ 7550 4200
Connection ~ 7450 4250
Wire Wire Line
	5100 4100 7550 4100
Connection ~ 7550 4100
Wire Wire Line
	4550 4050 3400 4050
$Comp
L GND #PWR07
U 1 1 5200B765
P 4550 5200
F 0 "#PWR07" H 4550 5200 30  0001 C CNN
F 1 "GND" H 4550 5130 30  0001 C CNN
F 2 "" H 4550 5200 60  0001 C CNN
F 3 "" H 4550 5200 60  0001 C CNN
	1    4550 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 5000 4550 5200
Wire Wire Line
	4550 4050 4550 4500
Connection ~ 4550 4050
Wire Wire Line
	3400 4150 5000 4150
Wire Wire Line
	3400 4250 4900 4250
Wire Wire Line
	5000 4150 5000 4650
Connection ~ 7450 4550
Wire Wire Line
	6650 4650 7550 4650
Wire Wire Line
	4900 4250 4900 4600
Connection ~ 7550 4650
$Comp
L MOSFET_N Q1
U 1 1 5200DC67
P 5750 4500
F 0 "Q1" H 5760 4670 60  0000 R CNN
F 1 "2N7002P" H 5760 4350 60  0000 R CNN
F 2 "SOT23GDS" H 5750 4500 60  0001 C CNN
F 3 "" H 5750 4500 60  0000 C CNN
	1    5750 4500
	1    0    0    -1  
$EndComp
$Comp
L MOSFET_N Q2
U 1 1 5200DC94
P 6150 4600
F 0 "Q2" H 6160 4770 60  0000 R CNN
F 1 "2N7002P" H 6160 4450 60  0000 R CNN
F 2 "SOT23GDS" H 6150 4600 60  0001 C CNN
F 3 "" H 6150 4600 60  0000 C CNN
	1    6150 4600
	1    0    0    -1  
$EndComp
$Comp
L R_PACK4 RP2
U 1 1 5200DCA6
P 6250 3850
F 0 "RP2" H 6250 4300 40  0000 C CNN
F 1 "4k7" H 6250 3800 40  0000 C CNN
F 2 "4X0603" H 6250 3850 60  0001 C CNN
F 3 "" H 6250 3850 60  0000 C CNN
	1    6250 3850
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR08
U 1 1 5200DCC6
P 3400 3300
F 0 "#PWR08" H 3400 3390 20  0001 C CNN
F 1 "+5V" H 3400 3390 30  0000 C CNN
F 2 "" H 3400 3300 60  0000 C CNN
F 3 "" H 3400 3300 60  0000 C CNN
	1    3400 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 3450 3400 3300
$Comp
L +5V #PWR09
U 1 1 5200DD09
P 6050 3550
F 0 "#PWR09" H 6050 3640 20  0001 C CNN
F 1 "+5V" H 6050 3640 30  0000 C CNN
F 2 "" H 6050 3550 60  0000 C CNN
F 3 "" H 6050 3550 60  0000 C CNN
	1    6050 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 3650 6200 3650
Wire Wire Line
	6050 3650 6050 3550
Connection ~ 6050 3650
Connection ~ 6100 3650
Connection ~ 6000 3650
Wire Wire Line
	6250 4400 6250 4050
Wire Wire Line
	6250 4050 6200 4050
Wire Wire Line
	5850 4300 5850 4050
Wire Wire Line
	5850 4050 5900 4050
Wire Wire Line
	5000 4500 5550 4500
Wire Wire Line
	4900 4600 5950 4600
Wire Wire Line
	6250 4800 6250 5200
Wire Wire Line
	5850 4700 5850 5200
Wire Wire Line
	5850 4300 6700 4300
Wire Wire Line
	6700 4300 6700 4550
Wire Wire Line
	6250 4350 6650 4350
Wire Wire Line
	6650 4350 6650 4650
Connection ~ 6250 4350
Connection ~ 5850 4300
Wire Wire Line
	5050 4050 5100 4050
Wire Wire Line
	5100 4050 5100 4100
Wire Wire Line
	6600 3600 6600 3550
Wire Wire Line
	6050 3550 6200 3550
Connection ~ 6050 3550
$Comp
L GND #PWR010
U 1 1 5200E14B
P 5300 5200
F 0 "#PWR010" H 5300 5200 30  0001 C CNN
F 1 "GND" H 5300 5130 30  0001 C CNN
F 2 "" H 5300 5200 60  0001 C CNN
F 3 "" H 5300 5200 60  0001 C CNN
	1    5300 5200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 5200E151
P 5000 5200
F 0 "#PWR011" H 5000 5200 30  0001 C CNN
F 1 "GND" H 5000 5130 30  0001 C CNN
F 2 "" H 5000 5200 60  0001 C CNN
F 3 "" H 5000 5200 60  0001 C CNN
	1    5000 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 5200 5000 5050
Wire Wire Line
	5300 5200 5300 5050
Wire Wire Line
	5300 4650 5300 4600
Connection ~ 5300 4600
Connection ~ 5000 4500
Wire Wire Line
	7300 3950 7450 3950
Wire Wire Line
	7550 3950 7700 3950
Wire Wire Line
	7700 3950 7700 3450
Wire Wire Line
	7700 3450 7550 3450
Connection ~ 7600 3950
Wire Wire Line
	7300 3950 7300 3450
Wire Wire Line
	7300 3450 7450 3450
Connection ~ 7400 3950
NoConn ~ 5100 5050
NoConn ~ 5200 5050
NoConn ~ 5200 4650
NoConn ~ 5100 4650
NoConn ~ 6000 4050
NoConn ~ 6100 4050
NoConn ~ 3400 4350
Wire Wire Line
	7250 4250 7250 4300
$Comp
L TST P2
U 1 1 52026528
P 7400 3950
F 0 "P2" H 7400 4250 40  0000 C CNN
F 1 "TST" H 7400 4200 30  0000 C CNN
F 2 "SOLDER_PAD2" H 7400 3950 60  0001 C CNN
F 3 "" H 7400 3950 60  0000 C CNN
	1    7400 3950
	1    0    0    -1  
$EndComp
$Comp
L AKL_5_2 J2
U 1 1 5200B5C2
P 8300 4400
F 0 "J2" H 8400 4395 60  0000 C CNN
F 1 "AKL_5_2" H 8285 4300 60  0000 C CNN
F 2 "AKL_5_2" H 8300 4400 60  0001 C CNN
F 3 "~" H 8300 4400 60  0000 C CNN
	1    8300 4400
	1    0    0    -1  
$EndComp
$Comp
L AKL_5_2 J3
U 1 1 5200B5AB
P 8300 4600
F 0 "J3" H 8400 4595 60  0000 C CNN
F 1 "AKL_5_2" H 8285 4500 60  0000 C CNN
F 2 "AKL_5_2" H 8300 4600 60  0001 C CNN
F 3 "~" H 8300 4600 60  0000 C CNN
	1    8300 4600
	1    0    0    -1  
$EndComp
$Comp
L TST P7
U 1 1 5202657F
P 7600 3950
F 0 "P7" H 7600 4250 40  0000 C CNN
F 1 "TST" H 7600 4200 30  0000 C CNN
F 2 "SOLDER_PAD2" H 7600 3950 60  0001 C CNN
F 3 "" H 7600 3950 60  0000 C CNN
	1    7600 3950
	1    0    0    -1  
$EndComp
$Comp
L TST P3
U 1 1 52026585
P 7550 4350
F 0 "P3" H 7550 4650 40  0000 C CNN
F 1 "TST" H 7550 4600 30  0000 C CNN
F 2 "SOLDER_PAD2" H 7550 4350 60  0001 C CNN
F 3 "" H 7550 4350 60  0000 C CNN
	1    7550 4350
	0    1    1    0   
$EndComp
$Comp
L TST P4
U 1 1 5202658B
P 7550 4450
F 0 "P4" H 7550 4750 40  0000 C CNN
F 1 "TST" H 7550 4700 30  0000 C CNN
F 2 "SOLDER_PAD2" H 7550 4450 60  0001 C CNN
F 3 "" H 7550 4450 60  0000 C CNN
	1    7550 4450
	0    1    1    0   
$EndComp
$Comp
L TST P5
U 1 1 5202659B
P 7550 4550
F 0 "P5" H 7550 4850 40  0000 C CNN
F 1 "TST" H 7550 4800 30  0000 C CNN
F 2 "SOLDER_PAD2" H 7550 4550 60  0001 C CNN
F 3 "" H 7550 4550 60  0000 C CNN
	1    7550 4550
	0    1    1    0   
$EndComp
$Comp
L TST P6
U 1 1 520265A1
P 7550 4650
F 0 "P6" H 7550 4950 40  0000 C CNN
F 1 "TST" H 7550 4900 30  0000 C CNN
F 2 "SOLDER_PAD2" H 7550 4650 60  0001 C CNN
F 3 "" H 7550 4650 60  0000 C CNN
	1    7550 4650
	0    1    1    0   
$EndComp
$Comp
L AGND #PWR012
U 1 1 522326CE
P 4700 2700
F 0 "#PWR012" H 4700 2700 40  0001 C CNN
F 1 "AGND" H 4700 2630 50  0000 C CNN
F 2 "" H 4700 2700 60  0000 C CNN
F 3 "" H 4700 2700 60  0000 C CNN
	1    4700 2700
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR L1
U 1 1 522326DD
P 4400 2550
F 0 "L1" V 4350 2550 40  0000 C CNN
F 1 "FB" V 4500 2550 40  0000 C CNN
F 2 "0603" H 4400 2550 60  0001 C CNN
F 3 "" H 4400 2550 60  0000 C CNN
	1    4400 2550
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR013
U 1 1 522326F6
P 4100 2700
F 0 "#PWR013" H 4100 2700 30  0001 C CNN
F 1 "GND" H 4100 2630 30  0001 C CNN
F 2 "" H 4100 2700 60  0001 C CNN
F 3 "" H 4100 2700 60  0001 C CNN
	1    4100 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 2700 4100 2550
Wire Wire Line
	4700 2700 4700 2550
$Comp
L AGND #PWR014
U 1 1 522327DA
P 7250 4300
F 0 "#PWR014" H 7250 4300 40  0001 C CNN
F 1 "AGND" H 7250 4230 50  0000 C CNN
F 2 "" H 7250 4300 60  0000 C CNN
F 3 "" H 7250 4300 60  0000 C CNN
	1    7250 4300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 523C317A
P 6250 5200
F 0 "#PWR015" H 6250 5200 30  0001 C CNN
F 1 "GND" H 6250 5130 30  0001 C CNN
F 2 "" H 6250 5200 60  0001 C CNN
F 3 "" H 6250 5200 60  0001 C CNN
	1    6250 5200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 523C3180
P 5850 5200
F 0 "#PWR016" H 5850 5200 30  0001 C CNN
F 1 "GND" H 5850 5130 30  0001 C CNN
F 2 "" H 5850 5200 60  0001 C CNN
F 3 "" H 5850 5200 60  0001 C CNN
	1    5850 5200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 523C3190
P 6600 3600
F 0 "#PWR017" H 6600 3600 30  0001 C CNN
F 1 "GND" H 6600 3530 30  0001 C CNN
F 2 "" H 6600 3600 60  0001 C CNN
F 3 "" H 6600 3600 60  0001 C CNN
	1    6600 3600
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 5200E08A
P 6400 3550
F 0 "C2" V 6550 3550 50  0000 L CNN
F 1 "100nF" V 6250 3450 50  0000 L CNN
F 2 "0603" H 6400 3550 60  0001 C CNN
F 3 "" H 6400 3550 60  0001 C CNN
	1    6400 3550
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
