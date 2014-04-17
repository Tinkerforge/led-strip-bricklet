function matlab_example_callback
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletLEDStrip;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'jHb'; % Change to your UID

    global NUM_LEDS;
    global r;
    global g;
    global b;
    global r_index;
    NUM_LEDS = 16;
    r = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];
    g = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];
    b = [0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0];
    r_index = 1;

    ipcon = IPConnection(); % Create IP connection
    led_strip = BrickletLEDStrip(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set frame duration to 50ms (20 frames per second)
    led_strip.setFrameDuration(50);

    % Register frame rendered callback to function cb_frame_rendered
    set(led_strip, 'FrameRenderedCallback', @(h, e) cb_frame_rendered(e));

    % Set initial rgb values to get started
    led_strip.setRGBValues(1, NUM_LEDS, r, g, b);

    input('Press any key to exit...\n', 's');
    ipcon.disconnect();
end

% Frame rendered callback, is called when a new frame was rendered
% We increase the index of one blue LED with every frame
function cb_frame_rendered(e)
    global NUM_LEDS;
    global r_index;
    global r;
    global g;
    global b;

    led_strip = e.getSource();
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
