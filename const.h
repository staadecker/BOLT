/*  Contains the constants used accross multiple files, especially the pins.
*/
#ifndef CONST
#define CONST


namespace constants {

  const int P_BUTTON_INTERRUPT = 2;
  const int P_BUTTON_DATA = 3;
  const int P_BUTTON_CLOCK = 4;

  const int P_LED_VCC = 5;
  const int P_LED_DATA = 8;
  const int P_LED_CLOCK = 12;
  const int P_LED_LATCH = 13;

  const bool IS_BUTTON_SHIELD_CONNECTED = false;
  const bool IS_SCREEN_CONNECTED = false;

  const uint8_t NUMBER_OF_LEDS = 16;
}

#endif
