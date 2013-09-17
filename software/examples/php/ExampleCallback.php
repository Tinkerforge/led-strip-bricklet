<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLEDStrip.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLEDStrip;

$host = 'localhost';
$port = 4223;
$uid = 'XYZ'; // Change to your UID
$NUM_LEDS = 16;
$r = array(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
$g = array(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
$b = array(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
$rIndex = 0;

// Frame rendered callback, is called when a new frame was rendered
// We increase the index of one blue LED with every frame
function cb_frame_rendered($length)
{
	global $NUM_LEDS, $rIndex, $r, $g, $b, $ledStrip;
	$b[$rIndex] = 0;
	if($rIndex == $NUM_LEDS-1) {
		$rIndex = 0;
	} else {
		$rIndex++;
	}
	$b[$rIndex] = 255;

	$ledStrip->setRGBValues(0, $NUM_LEDS, $r, $g, $b);
}

$ipcon = new IPConnection(); // Create IP connection
$ledStrip = new BrickletLEDStrip($uid, $ipcon); // Create device object

$ipcon->connect($host, $port); // Connect to brickd
// Don't use device before ipcon is connected

// Set frame duration to 50ms (20 frames per second)
$ledStrip->setFrameDuration(50);

// Register frame rendered callback to function cb_frame_rendered
$ledStrip->registerCallback(BrickletLEDStrip::CALLBACK_FRAME_RENDERED, 'cb_frame_rendered');

// Set initial rgb values to get started
$ledStrip->setRGBValues(0, $NUM_LEDS, $r, $g, $b);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
