import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletLEDStrip;

public class ExampleSimple {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your LED Strip Bricklet
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLEDStrip ls = new BrickletLEDStrip(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Set first 10 LEDs to green
		ls.setRGBValues(0, (short)10,
		                new short[]{(short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0},
		                new short[]{(short)255, (short)255, (short)255, (short)255, (short)255, (short)255, (short)255, (short)255, (short)255, (short)255, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0},
		                new short[]{(short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0, (short)0});

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
