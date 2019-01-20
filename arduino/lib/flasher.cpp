#include "flasher.h"

Flasher::Flasher(LedManager ledArg) : ledManager(ledArg) {}

void Flasher::switchState() {
    stateIsOn = !stateIsOn;

    //TODO optimize
    for (uint8_t ledNumber = 0; ledNumber < NUMBER_OF_LEDS; ledNumber++) {
        if (flashing[ledNumber]) {
            if (stateIsOn) {
                ledManager.turnOn(ledNumber);
            } else {
                ledManager.turnOff(ledNumber);
            }
        }
    }
    ledManager.shiftOut();
    nextRun = millis() + FLASHER_INTERVAL;
}


void Flasher::startFlashing(uint8_t ledNumber) {
    if (not flashing[ledNumber]) {
        flashing[ledNumber] = true;
        numberFlashing++;

        if (numberFlashing == 1) {
            threaderId = threader::addCallback(this);
        }
    }
}

void Flasher::stopFlashing(uint8_t ledNumber) {
    if (flashing[ledNumber]) {
        flashing[ledNumber] = false;
        numberFlashing--;
        threader::removeCallback(threaderId);
        ledManager.turnOff(ledNumber);
        ledManager.shiftOut();
    }
}

void Flasher::call() {
    if (millis() > nextRun) {
        switchState();
    }
}

