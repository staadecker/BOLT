# Bolt-Arduino

This folder contains the Arduino code for the BOLT project.

## The hardware setup

The Arduino controls 64 buttons using a 64-button shield. It controls the LEDs using eight 8-bit shift registers. It communicates to a bluetooth device with an external bluetooth chip.

## The code

The code that runs on the Arduino controls the LED's and registers the button presses. It also allows a device to connect via bluetooth. This device will be informed of button presses and will be able to send commands to turn the LEDs on or off.

### Modes

The code has two modes.
1. An offline simple reaction time tester than can be run without any phone.
2. A online mode where the arduino simply acts as a bridge between the buttons/LEDs and the phone.


