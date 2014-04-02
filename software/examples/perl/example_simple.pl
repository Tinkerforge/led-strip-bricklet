#!/usr/bin/perl  

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletLEDStrip;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'oud'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $led_strip = Tinkerforge::BrickletLEDStrip->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Set first 10 LEDs to green
my $r = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
my $g = [255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0];
my $b = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];

$led_strip->set_rgb_values(0, 10, $r, $g, $b);

print "Press any key to exit...\n";
<STDIN>;
$ipcon->disconnect();

