function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletLEDStrip;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your LED Strip Bricklet

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
    ls = handle(BrickletLEDStrip(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set frame duration to 50ms (20 frames per second)
    ls.setFrameDuration(50);

    % Register frame rendered callback to function cb_frame_rendered
    set(ls, 'FrameRenderedCallback', @(h, e) cb_frame_rendered(e));

    % Set initial rgb values to get started
    ls.setRGBValues(1, NUM_LEDS, r, g, b);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Use frame rendered callback to move the active LED every frame
function cb_frame_rendered(e)
    global NUM_LEDS;
    global r_index;
    global r;
    global g;
    global b;

    ls = e.getSource();
    b(r_index) = 1;

    if r_index == NUM_LEDS
        r_index = 1;
    else
        r_index = r_index + 1;
    end

    b(r_index) = 255;

    % Set new data for next render cycle
    ls.setRGBValues(1, NUM_LEDS, r, g, b);
end
