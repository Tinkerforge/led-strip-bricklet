#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletLEDStrip;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your LED Strip Bricklet

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $ls = Tinkerforge::BrickletLEDStrip->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Set first 10 LEDs to green
my $r = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
my $g = [255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0];
my $b = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];

$ls->set_rgb_values(0, 10, $r, $g, $b);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
