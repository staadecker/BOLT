#ifndef FLASHER_H
#define FLASHER_H

#include <USBAPI.h>
#include "constants.h"
#include "ledController.h"
#include "threader.h"

class Flasher : Thread {
    const unsigned int FLASHER_INTERVAL = 500;

    bool flashing[NUMBER_OF_LEDS] = {};
    bool stateIsOn = false;
    uint8_t numberFlashing = 0;
    unsigned long nextRun = millis();
    LedController &ledManager;

    void switchState();

public:
    explicit Flasher(LedController &led) : ledManager(led) {};

    void startFlashing(const uint8_t &ledNumber);

    void stopFlashing(const uint8_t &ledNumber);

    void runThread() override;
};

#endif
