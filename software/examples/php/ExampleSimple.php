<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLEDStrip.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLEDStrip;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your LED Strip Bricklet

$ipcon = new IPConnection(); // Create IP connection
$ls = new BrickletLEDStrip(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Set first 10 LEDs to green
$ls->setRGBValues(0, 10, array(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
                  array(255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0),
                  array(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
