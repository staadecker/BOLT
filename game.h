#ifndef GAME
#define GAME
#include "button.h"
#include "screen.h"
#include "led.h"
#include "clock.h"

unsigned long game_startTime;

int startGame(int gameTime){
  game_startTime = millis();
  
  int buttonsPressed = 0;

  clock_reset();
  
  while(true){
    clock_run();

    //Generate random button
    int buttonNumber = random(1, 65);

    //Turn on led
    setLedState(buttonNumber, STATE_ON);

    //Wait for press then turn of led
    button_wait(buttonNumber);
    setLedState(buttonNumber, STATE_OFF);

    //Increment counter
    buttonsPressed ++;
  }

  if (buttonsPressed == 0){
    return -1;
  }
  
  return (millis() - game_startTime) / buttonsPressed;
}


#endif
