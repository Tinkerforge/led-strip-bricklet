#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your LED Strip Bricklet

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_led_strip import BrickletLEDStrip

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    ls = BrickletLEDStrip(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Set first 10 LEDs to green
    r = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    g = [255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0]
    b = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    ls.set_rgb_values(0, 10, r, g, b)

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
