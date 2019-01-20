#ifndef FLASHER_H
#define FLASHER_H

#include <USBAPI.h>
#include "constants.h"
#include "led-manager.h"
#include "threader.h"

class Flasher : ThreaderCallback {
    const unsigned int FLASHER_INTERVAL = 500;

    bool flashing[NUMBER_OF_LEDS] = {};
    bool stateIsOn = false;
    uint8_t numberFlashing = 0;
    uint8_t threaderId = 0;
    unsigned long nextRun = millis();
    LedManager ledManager;

    void switchState();

public:
    explicit Flasher(LedManager led);

    void startFlashing(uint8_t ledNumber);

    void stopFlashing(uint8_t ledNumber);

    void call() override;
};

#endif
