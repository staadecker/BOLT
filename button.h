/* Manages interaction with the buttons */
#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

namespace button {
  bool isPressed(uint8_t buttonToCheck);
  void clearLast();
  void setup();
}

#endif
