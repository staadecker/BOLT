#include "flasher.h"

#include "led.h"
#include "constants.h"

namespace flasher {
namespace {
const unsigned int FLASHER_INTERVAL = 500;

    bool flashing[NUMBER_OF_LEDS];

bool currentFlashingState = LOW;

unsigned long nextRun = millis();

void flash() {
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
}

void startFlashing(uint8_t ledNumber) {
  flashing[ledNumber] = true;
}

void stopFlashing(uint8_t ledNumber) {
  flashing[ledNumber] = false;
  led::turnOff(ledNumber);
  led::shiftOut();
}

void checkFlash() {
  if (millis() > nextRun) {
    flash();
  }
}
}
