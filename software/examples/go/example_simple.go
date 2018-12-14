package main

import (
	"fmt"
"tinkerforge/ipconnection"
    "tinkerforge/led_strip_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your LED Strip Bricklet.

func main() {
	ipcon := ipconnection.New()
    defer ipcon.Close()
	ls, _ := led_strip_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
    defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	var r [16]uint8
	g :=  [16]uint8{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}
	var b [16]uint8

	ls.SetRGBValues(0, 10, r, g, b)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

	ipcon.Disconnect()
}
