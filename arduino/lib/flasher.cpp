#include "flasher.h"

#include "led.h"


void Flasher::flash() {
    currentFlashingState = !currentFlashingState;

    for (uint8_t led = 0; led < NUMBER_OF_LEDS; led++) {
        if (flashing[led]) {
            if (currentFlashingState) {
                led::turnOn(led);
            } else {
                led::turnOff(led);
            }
        }
    }
    led::shiftOut();
    nextRun = millis() + FLASHER_INTERVAL;
}


void Flasher::startFlashing(uint8_t ledNumber) {
    flashing[ledNumber] = true;
}

void Flasher::stopFlashing(uint8_t ledNumber) {
    flashing[ledNumber] = false;
    led::turnOff(ledNumber);
    led::shiftOut();
}

void Flasher::checkFlash() {
    if (millis() > nextRun) {
        flash();
    }
}

