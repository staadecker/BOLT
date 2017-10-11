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

  //Wait for center button to be pressed
  led::setState(0, led::STATE_FLASHING);
  button::wait(0);
  led::setState(0, led::STATE_OFF);

  //Start game
  game::start();
  helper::waitTime(5000);
}
