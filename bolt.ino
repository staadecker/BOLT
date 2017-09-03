#ifndef main.h
#define main.h

#include "button.h"
#include "led.h"
#include "screen.h"
#include "game.h"


void setup() {
  Serial.begin(115200);
  
  attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT), buttonCallback, FALLING);

  randomSeed(analogRead(0));

  setupClockThread();
}

void loop() {
  printToLCD("READY");
  setLedState(1, STATE_FLASHING);

  waitForButtonPress(1);

  printToLCD("3");
  delay(1000);
  printToLCD("2");
  delay(1000);
  printToLCD("1");
  delay(1000);

  Serial.println(startGame(30000));
}

#endif
