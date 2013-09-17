<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLEDStrip.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLEDStrip;

$host = 'localhost';
$port = 4223;
$uid = 'XYZ'; // Change to your UID

$ipcon = new IPConnection(); // Create IP connection
$ledStrip = new BrickletLEDStrip($uid, $ipcon); // Create device object

$ipcon->connect($host, $port); // Connect to brickd
// Don't use device before ipcon is connected

// Set first 10 LEDs to green
$r = array(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
$g = array(255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0);
$b = array(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
$ledStrip->setRGBValues(0, 10, $r, $g, $b);

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
