# Bluetooth protocol

## Format

Packet = Command byte + arguments seperated by commas + carriage return (ASCII 15)

Ex : C10,50/r

## Commands

| Command byte | Command description | Direction | Arguments |
|-----|-----|-----|-----|
| A | Acknowledge receiving a packet | Arduino to phone | None |
| B | Called when a button is pressed | Arduino to phone | Number of the button pressed (between 0-63)|
| D | Called to disconnect with arduino | Phone to arduino | None |
| L | Called to change state of led | Phone to arduino | 1) Number of LED to change <br> 2) State of led (off = 0; on = 1; flashing = 2)|
| S | Display to screen | Phone to arduino | Message to display to screen |
