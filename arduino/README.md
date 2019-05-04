# Bolt Arduino

This folder contains the Arduino code for the BOLT project.

# Get started (with CLion)

1. Edit `CMakeLists.txt` to match your software setup.

2. Edit `lib/constants.h` to match your hardware setup.

3. Run the `upload-main` run configuration in CLion to upload the code.

4. Use the Serial Monitor plugin to monitor the Arduino serial.

# Implementation Details

## The Hardware Setup

The Arduino controls 64 buttons using a 64-button shield. It controls the LEDs using eight 8-bit shift registers. It communicates to a bluetooth device with an external bluetooth chip.

## The Code

The code that runs on the Arduino controls the LED's and registers the button presses. It also allows a device to connect via bluetooth. This device will be informed of button presses and will be able to send commands to turn the LEDs on or off.

### Arduino states (modes)

1. **Start state.** The board is either waiting for the user to switch to the offline state by pressing the middle button (#0) or the online state (switched to when a bluetooth packet is received).

2. **Offline state.** The state is a simple game that measures the users reaction speed.

3. **Online mode.** In this state, the arduino simply acts as a bridge between the buttons/LEDs and the phone. The Arduino is listening for bluetooth commands sent from the phone.
