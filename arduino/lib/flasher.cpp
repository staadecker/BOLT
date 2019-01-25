#include "flasher.h"

void Flasher::switchFlashingState() {
    currentFlashState = !currentFlashState;

    for (unsigned char ledNumber = 0; ledNumber < NUMBER_OF_LEDS; ledNumber++) {
        if (flashingLEDs[ledNumber]) {
            if (currentFlashState) {
                ledManager.turnOnLed(ledNumber);
            } else {
                ledManager.turnOffLed(ledNumber);
            }
        }
    }

    ledManager.shiftOutLEDs();

    timeToNextFlash = millis() + FLASHING_INTERVAL;
}


void Flasher::startFlashingLED(const unsigned char ledNumber) {
    if (not flashingLEDs[ledNumber]) {
        flashingLEDs[ledNumber] = true;
        numberOfLEDsFlashing++;

        if (numberOfLEDsFlashing == 1) {
            timeToNextFlash = millis();
            runnablesManager::addRunnable(this);
        }
    }
}

void Flasher::stopFlashingLED(const unsigned char ledNumber) {
    if (flashingLEDs[ledNumber]) {
        flashingLEDs[ledNumber] = false;
        numberOfLEDsFlashing--;

        if (numberOfLEDsFlashing == 0) {
            runnablesManager::removeRunnable(this);
            ledManager.turnOffLed(ledNumber);
            ledManager.shiftOutLEDs();
        }
    }
}

void Flasher::onRun() {
    if (millis() >= timeToNextFlash) {
        switchFlashingState();
    }
}

