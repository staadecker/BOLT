#include "flasher.h"

#include "led.h"

Flasher::Flasher(Led ledArg) : led(ledArg) {}

void Flasher::flash() {
    currentFlashingState = !currentFlashingState;

    for (uint8_t ledNumber = 0; ledNumber < NUMBER_OF_LEDS; ledNumber++) {
        if (flashing[ledNumber]) {
            if (currentFlashingState) {
                led.turnOn(ledNumber);
            } else {
                led.turnOff(ledNumber);
            }
        }
    }
    led.shiftOut();
    nextRun = millis() + FLASHER_INTERVAL;
}


void Flasher::startFlashing(uint8_t ledNumber) {
    flashing[ledNumber] = true;
}

void Flasher::stopFlashing(uint8_t ledNumber) {
    flashing[ledNumber] = false;
    led.turnOff(ledNumber);
    led.shiftOut();
}

void Flasher::checkFlash() {
    if (millis() > nextRun) {
        flash();
    }
}

