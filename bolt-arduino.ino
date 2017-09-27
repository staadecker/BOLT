#include "button.h"
#include "led.h"
#include "screen.h"
#include "game.h"
#include "clock.h"
#include "const.h"
#include "controller.h"
#include "flasher.h"
#include "logger.h"


void setup() {
  Serial.begin(115200);

  //Attach interrupt for 64 button shield
  attachInterrupt(digitalPinToInterrupt(P_BUTTON_INTERRUPT), button_ISR, FALLING);

  //Generate new random seed
  randomSeed(analogRead(0));

  led_setup();
  flasher_setup();
  clock_setup();
  logger(LOGGER_TYPE_INFO, "main", "Setup done");

  //Starting procedure
  screen_display("READY");

  led_setState(0, LED_STATE_FLASHING);
  button_wait(0);
  led_setState(0, LED_STATE_OFF);

  game_start();
  logger(LOGGER_TYPE_INFO, "main", "Program done");
}

void loop() {}
