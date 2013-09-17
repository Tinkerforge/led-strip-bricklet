
#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_led_strip.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

int main() {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	LEDStrip led_strip;
	led_strip_create(&led_strip, UID, &ipcon); 

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		exit(1);
	}
	// Don't use device before ipcon is connected

	// Set first 10 LEDs to green
	uint8_t r[16] = {0};
	uint8_t g[16] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
	uint8_t b[16] = {0};
	led_strip_set_rgb_values(&led_strip, 0, 10, r, g, b);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
}
