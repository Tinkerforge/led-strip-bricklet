function octave_example_callback
    more off;
    
    HOST = "localhost";
    PORT = 4223;
    UID = "jHb"; % Change to your UID
    
    global NUM_LEDS;
    NUM_LEDS = 16;

    global r;
    global g;
    global b;
    global r_index;
    r = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];
    g = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];
    b = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];
    r_index = 1;
    
    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    global led_strip;
    led_strip = java_new("com.tinkerforge.BrickletLEDStrip", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set frame duration to 50ms (20 frames per second)
    led_strip.setFrameDuration(50);

    % Register frame rendered callback to function cb_frame_rendered
    led_strip.addFrameRenderedListener("cb_frame_rendered");

    % Set initial rgb values to get started
    led_strip.setRGBValues(1, NUM_LEDS, r, g, b);

    input("\nPress any key to exit...\n", "s");
    ipcon.disconnect();
end

% Frame rendered callback, is called when a new frame was rendered
% We increase the index of one blue LED with every frame
function cb_frame_rendered(length)
    global led_strip;
    global NUM_LEDS;
    global r_index;
    global r;
    global g;
    global b;
    b(r_index) = 1;
    if r_index == NUM_LEDS
        r_index = 1;
    else
        r_index = r_index + 1;
    end
    b(r_index) = 255;

    % Set new data for next render cycle
    led_strip.setRGBValues(1, NUM_LEDS, r, g, b);
end
