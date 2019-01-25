# Bluetooth protocol

This protocol dictates how the information being sent between the Arduino and the phone should be formatted.

## Arduino states

| State | When | What | Enter state | Leave state |
|-------|------|------|-------------|-------------|
| Online | While playing an online game | Device is only listensing for bluetooth packets | Receive a START packet | Receive and END packet <br> OR <br> Connection Lost (incomplete packet/no acknowledge) |
| Start | When user is not playing a game | Device is listening for bluetooth packets | Game ends | Game starts (online or offline) |
| Offline | When user is playing an offline game | Device is not listening for packets | Offline game started (middle button pressed) | Offline game ends |

## Packet Format

Packet's data is sent using ASCII. The maximum length of a packet is 255 bytes.

Packets start with ASCII byte 1 (start of header) and end with ASCII byte 4 (end of transmission).

After the ASCII byte 1, a series of commands is sent. Each command has a command byte and optionnally an argument. There may be multiple commands in one packet.

Each packet should be acknowledged by the receiver by replying with an ASCII byte 6 (acknowledged).

### Example transmission
Phone sends `0x01` + `L30S` + `0x03`. This turns on led number 30 and then shifts out to make the result appear.

Arduino replies with `0x06`.

### Arduino to Phone Commands

| Command byte | Hexadecimal | Decimal | Name | Description | Argument description |
|--------------|--------------|---------|------|-------------|----------------------|
| P | 0x50 | 80 | Button pressed | Called when a button is pressed | Number of the button pressed (between 0-63)|

### Phone to Arduino Commands

| Command byte | Hexadecimal | Decimal | Name | Description | Argument description |
|--------------|-------------|---------|------|-------------|----------------------|
| B | 0x42 | 66 | Begin | Called to start online game | None |
| E | 0x45 | 69 | End | Called to end online game | None |
| I | 0x49 | 73 | Led OFF | Called to turn off led | Number of LED to turn off (between 0-63)|
| O | 0x4F | 79 | Led ON | Called to turn on led | Number of LED to turn on (between 0-63)|
| S | 0x53 | 83 | Shift out | Called to actually display changes with led on/off | None |
