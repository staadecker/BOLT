#include "lib/game.h"
#include "lib/bluetooth.h"
#include "lib/button.h"
#include "lib/led.h"
#include "lib/logger.h"
#include "lib/screen.h"
#include "lib/flasher.h"
#include <Arduino.h>

void startReadyMode() {
  button::clearLast();
  screen::display("READY");
  flasher::startFlashing(0);
}

// Makes chasing lights on the outer circle
void bootUpSequence() {
  for (int i = 32; i < 64; i++) {
    led::turnOn(i);
    led::shiftOut();
    delay(70);
    led::turnOff(i);
  }

  for (int i = 0; i < 64; i++) {
    led::turnOff(i);
  }
  led::shiftOut();
}

void setup() {
  //Generate new random seed such that button sequence is different each time
  randomSeed(analogRead(0));

  //Setup
  bluetooth::setup();
  button::setup();
  led::setup();
  logger::log(logger::TYPE_INFO, "main", "Setup done");

  bootUpSequence();

  if (not bluetooth::shouldGoOnline()) {
    startReadyMode(); //Unless already connected start ready mode
  }

  while (true) {
    //If connected to bluetooth go in online mode
    if (bluetooth::shouldGoOnline()) {
      flasher::stopFlashing(0);
      screen::display("ONLINE");

      bluetooth::listen();

      //When disconnected
      startReadyMode();
    }

    //If middle button pressed go in offline mode
    if (button::isPressed(0)) {
      flasher::stopFlashing(0);

      game::start();

      //When game ends
      startReadyMode();
    }

    flasher::checkFlash(); // Will flash if should flash
  }

}

void loop(){}