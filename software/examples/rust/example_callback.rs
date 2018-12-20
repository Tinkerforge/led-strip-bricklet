use std::{error::Error, io, thread};
use tinkerforge::{ip_connection::IpConnection, led_strip_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your LED Strip Bricklet
const NUM_LEDS: u8 = 16;

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let ls = LedStripBricklet::new(UID, &ipcon); // Create device object

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd
                                          // Don't use device before ipcon is connected

    // Set frame duration to 50ms (20 frames per second)
    ls.set_frame_duration(50);

    let frame_rendered_receiver = ls.get_frame_rendered_callback_receiver();

    let r = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
    let g = [
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0,
    ];
    let mut b = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
    ls.set_rgb_values(0, NUM_LEDS, r, g, b);

    // Spawn thread to handle received events.
    // This thread ends when the `ls` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        let mut r_index = 0u8;
        for _frame_rendered in frame_rendered_receiver {
            b[r_index as usize] = 0;
            if r_index == NUM_LEDS - 1 {
                r_index = 0;
            } else {
                r_index += 1;
            }
            b[r_index as usize] = 255;

            // Set new data for next render cycle
            ls.set_rgb_values(0, NUM_LEDS, r, g, b);
        }
    });

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
