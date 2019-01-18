/*  Contains the constants used accross multiple files, especially the pins.
*/
#ifndef CONST_H
#define CONST_H

namespace constants {
const int P_BUTTON_INTERRUPT = 2;
const int P_BUTTON_DATA = 3;
const int P_BUTTON_CLOCK = 4;

const int P_LED_VCC = 5;
const int P_LED_DATA = 8;
const int P_LED_CLOCK = 12;
const int P_LED_LATCH = 7;

const int P_SOFTWARE_SERIAL_TX = 10;
const int P_SOFTWARE_SERIAL_RX = 9;

const bool IS_BUTTONS_CONNECTED = true;
const bool IS_SCREEN_CONNECTED = false;
const bool IS_DEBUGGING = true;
const bool IS_BLUETOOTH_CHIP_CONNECTED = true;

const uint8_t NUMBER_OF_LEDS = 64;
}

#endif
