#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your LED Strip Bricklet
NUM_LEDS = 16

r = [0]*NUM_LEDS
g = [0]*NUM_LEDS
b = [0]*NUM_LEDS
r_index = 0

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_led_strip import BrickletLEDStrip

# Use frame rendered callback to move the active LED every frame
def cb_frame_rendered(length, ls):
    global r_index

    b[r_index] = 0
    if r_index == NUM_LEDS-1:
        r_index = 0
    else:
        r_index += 1
    b[r_index] = 255

    # Set new data for next render cycle
    ls.set_rgb_values(0, NUM_LEDS, r, g, b)

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    ls = BrickletLEDStrip(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Set frame duration to 50ms (20 frames per second)
    ls.set_frame_duration(50)

    # Register frame rendered callback to function cb_frame_rendered
    ls.register_callback(ls.CALLBACK_FRAME_RENDERED,
                         lambda x: cb_frame_rendered(x, ls))

    # Set initial rgb values to get started
    ls.set_rgb_values(0, NUM_LEDS, r, g, b)

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
