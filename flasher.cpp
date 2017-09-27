#include "flasher.h"

#include "led.h"
#include <Thread.h>

bool flashingCurrentState = true;
bool flashed = false;

Thread flasherThread = Thread();

void flasher_callback() {
  Serial.println("Flasher thread running");

  //Variable should turn true if something flahed
  flashed = false;

  led_shiftOut();

  if (!flashed) {
    //Nothing is flashing anymore
    flasherThread.enabled = false;
  } else {
    //If it did flash switch flashing state
    flashingCurrentState = !flashingCurrentState;
  }
}

void flasher_setup() {
  flasherThread.setInterval(FLASHER_INTERVAL);
  flasherThread.onRun(flasher_callback);
  flasherThread.enabled = false;
}

