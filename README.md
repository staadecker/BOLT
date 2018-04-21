# BOLT (Arduino)

WARNING : This project is not finished or fully functionnal.

## The project

This repository contains the Arduino code for the BOLT project.

BOLT is a machine that tests your reflex speed. The machine has buttons which contain embedded LEDs. When on LED turns on the user must press the matching button. The machine then calcultes the user's reflexes based on the time it took the user to press the button.

The project has two components.
1. The arduino that controls the buttons and LED's (this repository)
2. An android phone which communicates with the Arduino and displays the results. More info [here](https://github.com/SUPERETDUPER/bolt-android).

## The hardware setup

The Arduino controls the 64 buttons using a 64-button shield. It controls the LEDs using eight 8-bit shift registers. It communicates to a bluetooth device with an external bluetooth chip.

## The code

The code that runs on the Arduino controls the LED's and registers the button presses. It also allows a device to connect via bluetooth. This device will be informed of button presses and will be able to send commands to turn the LEDs on or off.

### Bluetooth

The bluetooth communcations use packets. The packets are sent to and from the Arduino. Packet's format is defined in the `bluetooth-protocol.md` file.
