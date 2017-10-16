# Bluetooth protocol

## Arduino states

| State | When | What | Enter state | Leave state |
|-------|------|------|-------------|-------------|
| Online | While playing an online game | Device is only listensing for bluetooth packets | Receive a START packet | Receive and END packet <br> OR <br> Connection Lost (incomplete packet/no acknowledge) |
| Ready | When user is not playing a game | Device is listening for bluetooth packets | Game ends | Game starts |
| Offline | When user is playing an offline game | Device is not listening for packets | Offline game started (middle button pressed) | Offline game ends |

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
| B | 0x42 | 66 | Button pressed | Called when a button is pressed | Number of the button pressed (between 0-63)|

### Phone to Arduino

| Command byte | Hexadecimal | Decimal | Name | Description | Argument |
|--------------|-------------|---------|------|-------------|-----------|
| S | 0x53 | 83 | Start | Called to start online game | None |
| D | 0x44 | 68 | End | Called to end online game | None |
| L | 0x4C | 76 | Led ON | Called to turn on led | Number of LED to turn on (between 0-63)|
| O | 0x4F | 79 | Led OFF | Called to turn off led | Number of LED to turn off (between 0-63)|
