package main

import (
	"fmt"
    "github.com/Tinkerforge/go-api-bindings/ipconnection"
    "github.com/Tinkerforge/go-api-bindings/led_strip_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your LED Strip Bricklet.

const NUM_LEDS = 16

func main() {
	ipcon := ipconnection.New()
    defer ipcon.Close()
	ls, _ := led_strip_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
    defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Set frame duration to 50ms (20 frames per second)
	ls.SetFrameDuration(50)

	var r [16]uint8
	g  := [16]uint8{255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0}
	var b [16]uint8

	ls.SetRGBValues(0, 10, r, g, b)

	var rIndex uint8

	ls.RegisterFrameRenderedCallback(func(length uint16) {
		b[rIndex] = 0
		if rIndex == NUM_LEDS-1 {
			rIndex = 0
		} else {
			rIndex++
		}
		b[rIndex] = 255
		ls.SetRGBValues(0, NUM_LEDS, r, g, b)
	})

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

	ipcon.Disconnect()
}
