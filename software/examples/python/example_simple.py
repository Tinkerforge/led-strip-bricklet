#!/usr/bin/env python
# -*- coding: utf-8 -*-  

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change to your UID

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_ambient_light import AmbientLight

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    al = AmbientLight(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get current illuminance (unit is Lux/10)
    illuminance = al.get_illuminance()/10.0

    print('Illuminance: ' + str(illuminance) + ' Lux')

    raw_input('Press key to exit\n') # Use input() in Python 3
    ipcon.disconnect()
