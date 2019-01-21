#include "flasher.h"

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


void Flasher::startFlashing(const uint8_t &ledNumber) {
    if (not flashing[ledNumber]) {
        flashing[ledNumber] = true;
        numberFlashing++;

        if (numberFlashing == 1) {
            threadManager::addThread(this);
        }
    }
}

void Flasher::stopFlashing(const uint8_t &ledNumber) {
    if (flashing[ledNumber]) {
        flashing[ledNumber] = false;
        numberFlashing--;
        threadManager::removeThread(this);
        ledManager.turnOff(ledNumber);
        ledManager.shiftOut();
    }
}

void Flasher::runThread() {
    if (millis() > nextRun) {
        switchState();
    }
}

