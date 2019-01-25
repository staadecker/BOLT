#ifndef FLASHER_H
#define FLASHER_H

#include <USBAPI.h>
#include "constants.h"
#include "ledController.h"
#include "threader.h"

class Flasher : Runnable {
    const unsigned int FLASHING_INTERVAL = 500;

    bool flashingLEDs[NUMBER_OF_LEDS] = {}; //Which LEDs are flashing
    bool currentFlashState = false;
    unsigned char numberOfLEDsFlashing = 0;
    unsigned long timeToNextFlash = millis();

    LedController &ledManager;

    void switchFlashingState();

public:
    explicit Flasher(LedController &led) : ledManager(led) {};

    void startFlashingLED(unsigned char ledNumber);

    void stopFlashingLED(unsigned char ledNumber);

    void onRun() override;
};

#endif
