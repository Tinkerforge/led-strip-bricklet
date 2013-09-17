#!/bin/sh
# connects to localhost:4223 by default, use --host and --port to change it

# change to your UID
uid=XYZ
r=0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
g=255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0
b=0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

# Set first 10 LEDs to green
tinkerforge call led-strip-bricklet $uid set-rgb-values 0 10 $r $g $b
