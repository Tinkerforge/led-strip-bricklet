using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID
	private static byte NUM_LEDS = 16;
	private static byte[] r = new byte[] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	private static byte[] g = new byte[] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	private static byte[] b = new byte[] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	private static int rIndex = 0;


	// Frame rendered callback, is called when a new frame was rendered
	// We increase the index of one blue LED with every frame
	static void FrameRenderedCB(BrickletLEDStrip sender, int length)
	{
		b[rIndex] = 0;
		if(rIndex == NUM_LEDS-1) {
			rIndex = 0;
		} else {
			rIndex++;
		}
		b[rIndex] = 255;

		// Set new data for next render cycle
		sender.SetRGBValues(0, NUM_LEDS, r, g, b);
	}

	static void Main() 
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLEDStrip ledStrip = new BrickletLEDStrip(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Set frame duration to 50ms (20 frames per second)
		ledStrip.SetFrameDuration(50);

		// Register frame rendered callback to function FrameRenderedCB
		ledStrip.FrameRendered += FrameRenderedCB;

		// Set initial rgb values to get started
		ledStrip.SetRGBValues(0, NUM_LEDS, r, g, b);

		System.Console.WriteLine("Press enter to exit");
		System.Console.ReadLine();
		ipcon.Disconnect();
	}
}
