/* Manages interaction with the buttons */
#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

namespace button {

  void isr();
  bool isButtonPressed(uint8_t buttonNumber);
  void wait(uint8_t buttonNumber);

}

#endif
