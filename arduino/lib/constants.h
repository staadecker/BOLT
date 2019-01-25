/*  Contains the constants used accross multiple files, especially the pins.
*/
#ifndef CONST_H
#define CONST_H

const bool IS_BUTTONS_CONNECTED = false;
const bool IS_SCREEN_CONNECTED = false;
const bool IS_BLUETOOTH_CHIP_CONNECTED = false;
const bool IS_LED_CONNECTED = false;

const unsigned char NUMBER_OF_LEDS = 64;

const int PIN_BUTTON_SHIELD_INTERRUPT = 2;
const int PIN_BUTTON_SHIELD_DATA = 3;
const int PIN_BUTTON_SHIELD_CLOCK = 4;

const int PIN_LED_SHIFT_REG_VCC = 5;
const int PIN_LED_SHIFT_REG_DATA = 8;
const int PIN_LED_SHIFT_REG_CLOCK = 12;
const int PIN_LED_SHIFT_REG_LATCH = 7;

const int PIN_BLUETOOTH_SERIAL_TX = 10;
const int PIN_BLUETOOTH_SERIAL_RX = 9;


#endif
