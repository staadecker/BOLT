#ifndef FLASHER_H
#define FLASHER_H

#include <Arduino.h>
#include "constants.h"

class Flasher {
    const unsigned int FLASHER_INTERVAL = 500;

    bool flashing[NUMBER_OF_LEDS] = {};
    bool currentFlashingState = LOW;
    unsigned long nextRun = millis();

    void flash();

public:
    void startFlashing(uint8_t ledNumber);

    void stopFlashing(uint8_t ledNumber);

    void checkFlash();
};

#endif
