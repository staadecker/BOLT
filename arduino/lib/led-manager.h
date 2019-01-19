#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include "constants.h"

class LedManager {
    const uint8_t VCC_PWM = 95; //Value to adjust for optimal 2V output voltage

    // Array keeping track of states
    uint8_t states[NUMBER_OF_LEDS] = {};

public:
    LedManager();

    void turnOn(uint8_t ledNumber);

    void turnOff(uint8_t ledNumber);

    void shiftOut();
};

#endif
