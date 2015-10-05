<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLEDStrip.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLEDStrip;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change to your UID
const NUM_LEDS = 16;

$r = array(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
$g = array(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
$b = array(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
$rIndex = 0;

// Use frame rendered callback to move the active LED every frame
function cb_frameRendered($length)
{
    global $rIndex, $r, $g, $b, $ls;

    $b[$rIndex] = 0;

    if ($rIndex == NUM_LEDS - 1) {
        $rIndex = 0;
    } else {
        $rIndex++;
    }

    $b[$rIndex] = 255;

    // Set new data for next render cycle
    $ls->setRGBValues(0, NUM_LEDS, $r, $g, $b);
}

$ipcon = new IPConnection(); // Create IP connection
$ls = new BrickletLEDStrip(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Set frame duration to 50ms (20 frames per second)
$ls->setFrameDuration(50);

// Register frame rendered callback to function cb_frameRendered
$ls->registerCallback(BrickletLEDStrip::CALLBACK_FRAME_RENDERED, 'cb_frameRendered');

// Set initial rgb values to get started
$ls->setRGBValues(0, NUM_LEDS, $r, $g, $b);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
