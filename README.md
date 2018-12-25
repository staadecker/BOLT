# BOLT (Arduino)

WARNING : This project is not finished or fully functionnal.

## The project

This repository contains the Arduino code for the BOLT project.

BOLT is a machine that tests your reflex speed. The machine has buttons which contain embedded LEDs. When an LED turns on, the user must press the matching button. The machine then calculates the user's reflexes based on the time it took the user to press the button.

The project has two components.
1. The arduino that controls the buttons and LED's (this repository)
2. An android phone which communicates with the Arduino and displays the results. See code [here](https://github.com/SUPERETDUPER/bolt-android).

## The hardware setup

The Arduino controls 64 buttons using a 64-button shield. It controls the LEDs using eight 8-bit shift registers. It communicates to a bluetooth device with an external bluetooth chip.

## The code

The code that runs on the Arduino controls the LED's and registers the button presses. It also allows a device to connect via bluetooth. This device will be informed of button presses and will be able to send commands to turn the LEDs on or off.

### Modes

The code has two modes.
1. An offline simple reaction time tester than can be run without any phone.
2. A online mode where the arduino simply acts as a bridge between the buttons/LEDs and the phone.

### Bluetooth

To communicate via bluetooth the Arduino and phone send each other packets. The packet's format is defined in the `bluetooth-protocol.md` file.

