# Bluetooth protocol

## Packet Format

Command byte + argument + carriage return (ASCII 15)

Everything in ASCII

Example : `L30/r` turns on led number 30

## Commands
### Arduino to Phone

| Command byte | Hexadecimal | Decimal | Name | Description | Argument |
|--------------|-------------|---------|------|-------------|-----------|
| A | 0x41 | 65 |Acknowledge | Acknowledge receiving a packet | None |
| B | 0x42 | 66 |Button pressed | Called when a button is pressed | Number of the button pressed (between 0-63)|

### Phone to Arduino

| Command byte | Hexadecimal | Decimal | Name | Description | Argument |
|--------------|-------------|---------|------|-------------|-----------|
| D | 0x44 | 68 | Disconnect | Called to disconnect with arduino | None |
| L | 0x4C | 76 | Led ON | Called to turn on led| Number of LED to turn on (between 0-63)|
| O | 0x4F | 79 | Led OFF | Called to turn off led | Number of LED to turn off (between 0-63)|
