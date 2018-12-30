t#include "game.h"
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
}

void runMain() {
  //Generate new random seed
  randomSeed(analogRead(0));

  //Setup
  bluetooth::setup();
  button::setup();
  led::setup();
  logger::log(logger::TYPE_INFO, "main", "Setup done");

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
