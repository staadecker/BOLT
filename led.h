#ifndef LED_H
#define LED_H

#include <Arduino.h>

namespace led {
  //Value to adjust for optimal output voltage
  const uint8_t VCC_PWM = 105;

  //0 is always default value
  const uint8_t STATE_OFF = 0;
  const uint8_t STATE_ON = 1;
  const uint8_t STATE_FLASHING = 2;

  void setup();
  void flash();
  void setState(uint8_t led, uint8_t state);
}

#endif
