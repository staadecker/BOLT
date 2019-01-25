#ifndef LED_H
#define LED_H


#include "constants.h"

class LedController {
    const unsigned char PWM_VALUE_FOR_VCC_PIN = 95; //Value to adjust for optimal 2V output voltage


    bool ledStates[NUMBER_OF_LEDS] = {};

public:
    LedController();

    void turnOnLed(unsigned char ledNumber);

    void turnOffLed(unsigned char ledNumber);

    void shiftOutLEDs();
};

#endif
