# Bluetooth protocol

## Packet Format

Everything in ASCII

Send : Start of text (ASCII 2) + Command byte + argument + End of text (ASCII 3)

Receive : Acknowledge (ASCII 6)

### Example
`0x02` + `L30` + `0x03` turns on led number 30. Replies with `0x06`

## Commands
### Arduino to Phone

| Command byte | Hexadecimal | Decimal | Name | Description | Argument |
|--------------|-------------|---------|------|-------------|-----------|
| R | 0x41 | 65 | Request acknowledge | Used to check connection | None |
| B | 0x42 | 66 | Button pressed | Called when a button is pressed | Number of the button pressed (between 0-63)|

### Phone to Arduino

| Command byte | Hexadecimal | Decimal | Name | Description | Argument |
|--------------|-------------|---------|------|-------------|-----------|
| D | 0x44 | 68 | Disconnect | Called to disconnect with arduino | None |
| L | 0x4C | 76 | Led ON | Called to turn on led | Number of LED to turn on (between 0-63)|
| O | 0x4F | 79 | Led OFF | Called to turn off led | Number of LED to turn off (between 0-63)|
