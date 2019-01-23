#ifndef LED_H
#define LED_H


#include "constants.h"

class LedController {
    const unsigned char VCC_PWM = 95; //Value to adjust for optimal 2V output voltage

    // Array keeping track of states
    bool states[NUMBER_OF_LEDS] = {}; //TODO switch to boolean

public:
    LedController();

    void turnOn(const unsigned char &ledNumber);

    void turnOff(const unsigned char &ledNumber);

    void shiftOut();
};

#endif
