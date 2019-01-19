#ifndef FLASHER_H
#define FLASHER_H

#include <Arduino.h>
#include "constants.h"
#include "led.h"

class Flasher {
    const unsigned int FLASHER_INTERVAL = 500;

    bool flashing[NUMBER_OF_LEDS] = {};
    bool currentFlashingState = LOW;
    unsigned long nextRun = millis();
    Led led;

    void flash();

public:
    Flasher(Led led);

    void startFlashing(uint8_t ledNumber);

    void stopFlashing(uint8_t ledNumber);

    void checkFlash();
};

#endif
