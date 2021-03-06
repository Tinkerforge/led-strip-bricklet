import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletLEDStrip;

public class ExampleCallback {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your LED Strip Bricklet
	private static final String UID = "XYZ";
	private static final int NUM_LEDS = 16;
	private static int rIndex = 0;
	private static short[] r = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	private static short[] g = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	private static short[] b = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		// Note: Declare ls as final, so the listener can access it
		final BrickletLEDStrip ls = new BrickletLEDStrip(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Set frame duration to 50ms (20 frames per second)
		ls.setFrameDuration(50);

		// Use frame rendered callback to move the active LED every frame
		ls.addFrameRenderedListener(new BrickletLEDStrip.FrameRenderedListener() {
			public void frameRendered(int length) {
				b[rIndex] = 0;
				if(rIndex == NUM_LEDS-1) {
					rIndex = 0;
				} else {
					rIndex++;
				}
				b[rIndex] = 255;

				// Set new data for next render cycle
				try {
					ls.setRGBValues(0, (short)NUM_LEDS, r, g, b);
				} catch(Exception e) {
					System.out.println(e);
				}
			}
		});

		// Set initial rgb values to get started
		ls.setRGBValues(0, (short)NUM_LEDS, r, g, b);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
