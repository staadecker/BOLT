#ifndef LED_H
#define LED_H

#include <USBAPI.h>
#include "constants.h"

class LedController {
    const uint8_t VCC_PWM = 95; //Value to adjust for optimal 2V output voltage

    // Array keeping track of states
    uint8_t states[NUMBER_OF_LEDS] = {};

public:
    LedController();

    void turnOn(const uint8_t &ledNumber);

    void turnOff(const uint8_t &ledNumber);

    void shiftOut();
};

#endif
