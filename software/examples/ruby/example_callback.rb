#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_led_strip'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your LED Strip Bricklet
NUM_LEDS = 16

r = [0]*NUM_LEDS
g = [0]*NUM_LEDS
b = [0]*NUM_LEDS
r_index = 0

ipcon = IPConnection.new # Create IP connection
ls = BrickletLEDStrip.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Set frame duration to 50ms (20 frames per second)
ls.set_frame_duration 50

# Use frame rendered callback to move the active LED every frame
ls.register_callback(BrickletLEDStrip::CALLBACK_FRAME_RENDERED) do |length|
  b[r_index] = 0
  if(r_index == NUM_LEDS-1)
    r_index = 0
  else
    r_index += 1
  end
  b[r_index] = 255

  # Set new data for next render cycle
  ls.set_rgb_values 0, NUM_LEDS, r, g, b
end

# Set initial rgb values to get started
ls.set_rgb_values(0, NUM_LEDS, r, g, b)

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
