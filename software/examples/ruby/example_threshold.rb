#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_ambient_light'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = '7tS' # Change to your UID

ipcon = IPConnection.new # Create IP connection
al = BrickletAmbientLight.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
al.set_debounce_period 10000

# Register threshold reached callback for illuminance greater than 200 Lux
al.register_callback(BrickletAmbientLight::CALLBACK_ILLUMINANCE_REACHED) do |illuminance|
  puts "We have #{illuminance/10.0} Lux."
  puts "Too bright, close the curtains!"
end

# Configure threshold for "greater than 200 Lux" (unit is Lux/10)
al.set_illuminance_callback_threshold '>', 200*10, 0

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
