#include "game.h"
#include "main.h"
#include "bluetooth.h"
#include "button.h"
#include "led.h"
#include "logger.h"
#include "screen.h"
#include "flasher.h"
#include <Arduino.h>

namespace main {
namespace {
void startReadyMode() {
  button::clearLast();
  screen::display("READY");
  flasher::startFlashing(0);
}

void bootUpSequence() {
  led::turnOn(0);
  led::shiftOut();

  delay(300);

  led::turnOff(0);
  for (int i = 1; i < 8; i++) {
    led::turnOn(i);
  }
  led::shiftOut();

  delay(180);

  for (int i = 1; i < 8; i++) {
    led::turnOff(i);
  }
  for (int i = 8; i < 16; i++) {
    led::turnOn(i);
  }
  led::shiftOut();

  delay(120);

  for (int i = 8; i < 16; i++) {
    led::turnOff(i);
  }
  for (int i = 16; i < 32; i++) {
    led::turnOn(i);
  }
  led::shiftOut();

  delay(50);

  for (int i = 16; i < 32; i++) {
    led::turnOff(i);
  }
  for (int i = 32; i < 64; i++) {
    led::turnOn(i);
  }
  led::shiftOut();

  for (int i = 0; i < 64; i++) {
    led::turnOff(i);
  }

  led::shiftOut();
}
}

void runMain() {
  //Generate new random seed
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
}
