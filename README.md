# BOLT

BOLT is a machine that measures your reaction speed. The machine has 64 buttons which contain embedded LEDs. When an LED turns on, the user must press the matching button. The machine then calculates the user's reaction speed based on the time it took the user to press the button.

The project has two components.
- `arduino` : The arduino code that controls the buttons and LED's
- `bolt-mobile` : The Flutter code that uses a phone to communicate with the Arduino and displays the results\

This project reached a semi-functionning state but was not fully finished.

### Bluetooth

To communicate the Arduino and phone send each other packets over bluetooth. The packets' format is defined in `bluetooth-protocol.md`.

