#ifndef LED_H
#define LED_H

#include <Arduino.h>

namespace led {
  //Value to adjust for optimal output voltage
  const uint8_t VCC_PWM = 105;

  void setup();
  void turnOn(uint8_t ledNumber);
  void turnOff(uint8_t ledNumber);
}

#endif
