function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletLEDStrip;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'jHb'; % Change to your UID

    ipcon = IPConnection(); % Create IP connection
    led_strip = BrickletLEDStrip(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set first 10 LEDs to green
    r = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];
    g = [255 255 255 255 255 255 255 255 255 255 0 0 0 0 0 0];
    b = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];
    led_strip.setRGBValues(0, 10, r, g, b);

    input('Press any key to exit...\n', 's');
    ipcon.disconnect();
end
