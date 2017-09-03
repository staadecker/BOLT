#ifndef game.h
#define game.h
#include "button.h"
#include "screen.h"
#include "led.h"

unsigned long startTime;

startGame(int gameTime){
  startTime = millis();
  
  int buttonsPressed = 0;

  startClock();
  
  while(true){

    //If time elapsed exit
    if ((millis() - startTime) > gameTime){
      timeThread.enabled = false;
      break;
    }

    //Generate random button
    int buttonNumber = random(1, 65);

    //Turn on led
    setLedState(buttonNumber, STATE_ON);

    //Wait for press then turn of led
    waitForButtonPress(buttonNumber);
    setLedState(buttonNumber, STATE_OFF);

    //Increment counter
    buttonsPressed ++;
  }

  if (buttonsPressed == 0){
    return "No buttons pressed";
  }
  
  return (millis() - startTime) / buttonsPressed;
}


#endif
