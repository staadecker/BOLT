#include "button.h"
#include "led.h"
#include "screen.h"
#include "game.h"
#include "timer.h"
#include "const.h"
#include "controller.h"
#include "flasher.h"
#include "logger.h"


void setup() {
  Serial.begin(115200);

  //Attach interrupt for 64 button shield
  attachInterrupt(digitalPinToInterrupt(constants::P_BUTTON_INTERRUPT), button::isr, FALLING);

  //Generate new random seed
  randomSeed(analogRead(0));

  //Setup
  led::setupLed();
  flasher::setupFlasher();
  timer::setupTimer();
  logger::logger(logger::TYPE_INFO, "main", "Setup done");
}


void loop() {
  //Starting procedure
  screen::displayToScreen("READY");

  //Wait for center button to be pressed
  led::setState(0, led::STATE_FLASHING);
  button::wait(0);
  led::setState(0, led::STATE_OFF);

  //Start game
  game::start();
  exit(0);
}
