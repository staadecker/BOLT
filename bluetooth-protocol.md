# Bluetooth protocol

## Packet Format

Command byte + argument + carriage return (ASCII 15)

Example : `L30/r` turns on led number 30

## Commands
### Arduino to Phone

| Command byte | Name | Description | Argument |
|--------------|------|-------------|-----------|
| A | Acknowledge | Acknowledge receiving a packet | None |
| B | Button pressed | Called when a button is pressed | Number of the button pressed (between 0-63)|

### Phone to Arduino

| Command byte | Name | Description | Argument |
|--------------|------|-------------|-----------|
| D | Disconnect | Called to disconnect with arduino | None |
| L | Led ON | Called to turn on led| Number of LED to turn on (between 0-63)|
| O | Led OFF | Called to turn off led | Number of LED to turn off (between 0-63)|
