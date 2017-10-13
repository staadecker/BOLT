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
  button::setup();
  led::setup();
  flasher::setup();
  timer::setup();
  logger::log(logger::TYPE_INFO, "main", "Setup done");
}


void loop() {
  screen::display("READY");
  led::setState(0, led::STATE_FLASHING);

  while (true) {
    controller::run(); //Run controller

    led::setState(0, led::STATE_OFF);

    if (bluetooth::isConnected()) {
      //Start online mode
    } else if (button::isPressed(0)) {
      game::start(); //Start offline game
    }
  }
  helper::waitTime(5000);
}
