#ifndef BOLTARDUINO
#define BOLTARDUINO

#include "button.h"
#include "led.h"
#include "screen.h"
#include "game.h"
#include "const.h"
#include "clock.h"


void setup() {
  Serial.begin(115200);

  //Attach interrupt for 64 button shield 
  attachInterrupt(digitalPinToInterrupt(P_BUTTON_INTERRUPT), button_ISR, FALLING);

  //Generate new random seed
  randomSeed(analogRead(0));
  
  clock_setup();
}

void loop() {
  screen_display("READY");
  setLedState(1, STATE_FLASHING);

  button_wait(1);

  screen_display("3");
  delay(1000);
  screen_display("2");
  delay(1000);
  screen_display("1");
  delay(1000);

  Serial.println(startGame(30000));
}

#endif
