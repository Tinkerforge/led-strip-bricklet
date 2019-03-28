function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your LED Strip Bricklet

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    ls = javaObject("com.tinkerforge.BrickletLEDStrip", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set first 10 LEDs to green
    ls.setRGBValues(0, 10, [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0], ...
                    [255 255 255 255 255 255 255 255 255 255 0 0 0 0 0 0], ...
                    [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0]);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end
