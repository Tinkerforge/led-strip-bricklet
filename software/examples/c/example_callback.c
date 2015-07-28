#include <stdio.h>

#include "ip_connection.h"
#include "bricklet_led_strip.h"

#define HOST "localhost"
#define PORT 4223
#define UID "XYZ" // Change to your UID

#define NUM_LEDS 16

uint8_t r[NUM_LEDS] = {0};
uint8_t g[NUM_LEDS] = {0};
uint8_t b[NUM_LEDS] = {0};
uint8_t r_index = 0;

// Frame rendered callback, is called when a new frame was rendered
// We increase the index of one blue LED with every frame
void cb_frame_rendered(uint16_t length, void *user_data) {
	(void)length; // avoid unused parameter warning

	LEDStrip *ls = (LEDStrip *)user_data;

	b[r_index] = 0;
	if(r_index == NUM_LEDS-1) {
		r_index = 0;
	} else {
		r_index++;
	}
	b[r_index] = 255;

	// Set new data for next render cycle
	led_strip_set_rgb_values(ls, 0, NUM_LEDS, r, g, b);
}

int main() {
	// Create IP connection
	IPConnection ipcon;
	ipcon_create(&ipcon);

	// Create device object
	LEDStrip ls;
	led_strip_create(&ls, UID, &ipcon);

	// Connect to brickd
	if(ipcon_connect(&ipcon, HOST, PORT) < 0) {
		fprintf(stderr, "Could not connect\n");
		exit(1);
	}
	// Don't use device before ipcon is connected
	
	// Set frame duration to 50ms (20 frames per second)
	led_strip_set_frame_duration(&ls, 50);

	// Register frame rendered callback to function cb_frame_rendered
	led_strip_register_callback(&ls,
	                            LED_STRIP_CALLBACK_FRAME_RENDERED,
	                            (void *)cb_frame_rendered,
	                            (void *)&ls);

	// Set initial rgb values to get started
	led_strip_set_rgb_values(&ls, 0, NUM_LEDS, r, g, b);

	printf("Press key to exit\n");
	getchar();
	ipcon_destroy(&ipcon); // Calls ipcon_disconnect internally
}
