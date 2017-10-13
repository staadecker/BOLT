#include "button.h"
#include "led.h"
#include "screen.h"
#include "game.h"
#include "timer.h"
#include "const.h"
#include "controller.h"
#include "flasher.h"
#include "logger.h"
#include "helper.h"
#include "bluetooth.h"


void setup() {
  Serial.begin(115200);

  //Generate new random seed
  randomSeed(analogRead(0));

  //Setup
  bluetooth:setup();
  button::setup();
  led::setup();
  flasher::setup();
  timer::setup();
  logger::log(logger::TYPE_INFO, "main", "Setup done");
}


void loop() {
  screen::display("READY");
  flasher::flash(0);

  while (true) {
    controller::run(); //Run controller

    if (bluetooth::isConnected()) {
      flasher::stopFlashing(0);
      //TODO : Start online mode
    } else if (button::isPressed(0)) {
      flasher::stopFlashing(0);
      game::start(); //Start offline game      
    }
  }
  helper::waitTime(5000);
}
