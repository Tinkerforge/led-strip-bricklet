#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your LED Strip Bricklet

# Set first 10 LEDs to green
tinkerforge call led-strip-bricklet $uid set-rgb-values 0 10 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 255,255,255,255,255,255,255,255,255,255,0,0,0,0,0,0 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
