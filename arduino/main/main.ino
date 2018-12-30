#include "button.h"
#include "led.h"
#include "screen.h"
#include "game.h"
#include "timer.h"
#include "constants.h"
#include "flasher.h"
#include "logger.h"
#include "bluetooth.h"


void setup() {
  Serial.begin(9600);

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
}


void loop() {
  //If connected to bluetooth go in online mode
  if (bluetooth::shouldGoOnline()) {
    flasher::stopFlashing(0);
    screen::display("ONLINE");

    bluetooth::listen();

    startReadyMode();
  }

  //If middle button pressed go in offline mode
  //if (button::isPressed(0)) {
    //flasher::stopFlashing(0);

    //game::start();

    //startReadyMode();
  //}

  flasher::checkFlash(); // Will flash if should flash
}

void startReadyMode() {
  button::clearLast();
  screen::display("READY");
  flasher::startFlashing(0);
}
