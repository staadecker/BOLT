/* Manages interaction with the buttons */
#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

namespace button {
  bool isPressed(uint8_t buttonToCheck);
  void wait(uint8_t buttonNumber);
  void setup();
}

#endif
