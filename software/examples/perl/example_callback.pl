#!/usr/bin/perl  

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletLEDStrip;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'jEE'; # Change to your UID

use constant NUM_LEDS => 16;

our $r = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
our $g = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
our $b = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
our $r_index = 0;

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
our $led_strip = Tinkerforge::BrickletLEDStrip->new(&UID, $ipcon); # Create device object

# Frame rendered callback, is called when a new frame was rendered
# We increase the index of one blue LED with every frame
sub cb_frame_rendered
{
    my ($length) = @_;

    @{$b}[$r_index] = 0;
    if ($r_index == NUM_LEDS-1)
    {
        $r_index = 0;
    }
    else
    {
        $r_index += 1;
    }
    @{$b}[$r_index] = 255;

    # Set new data for next render cycle
    $led_strip->set_rgb_values(0, NUM_LEDS, $r, $g, $b);
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Set frame duration to 50ms (20 frames per second)
$led_strip->set_frame_duration(50);

# Register frame rendered callback to function cb_frame_rendered
$led_strip->register_callback($led_strip->CALLBACK_FRAME_RENDERED, 'cb_frame_rendered');

# Set initial rgb values to get started
$led_strip->set_rgb_values(0, NUM_LEDS, $r, $g, $b);

print "\nPress any key to exit...\n";
<STDIN>;
$ipcon->disconnect();

