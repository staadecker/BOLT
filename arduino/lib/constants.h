/*  Contains the constants used accross multiple files, especially the pins.
*/
#ifndef CONST_H
#define CONST_H

//Toggle to enable different features
#define IS_BUTTONS_CONNECTED false // When false button presses are received from Serial.
#define IS_BLUETOOTH_CHIP_CONNECTED true // When true checks for start packets allowing online mode.
#define IS_LED_CONNECTED false // When true will run boot sequence

#define NUMBER_OF_LEDS 64

#define PIN_BUTTON_SHIELD_INTERRUPT 2
#define PIN_BUTTON_SHIELD_DATA 3
#define PIN_BUTTON_SHIELD_CLOCK  4

#define PIN_LED_SHIFT_REG_VCC  5
#define PIN_LED_SHIFT_REG_DATA 8
#define PIN_LED_SHIFT_REG_CLOCK  12
#define PIN_LED_SHIFT_REG_LATCH  7

#define PIN_BLUETOOTH_SERIAL_RX 9 // Connects to TXD on BT chip
#define PIN_BLUETOOTH_SERIAL_TX  10 // Connects to RXD on BT chip


#endif
