#ifndef LED_H
#define LED_H

#include <Arduino.h>

namespace led {
  
  

  void setup();
  void turnOn(uint8_t ledNumber);
  void turnOff(uint8_t ledNumber);
}

#endif
