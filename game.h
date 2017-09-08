#ifndef GAME
#define GAME
#include "button.h"
#include "screen.h"
#include "led.h"
#include "clock.h"
#include "const.h"

int game_runMain(int gameTime){
  unsigned long startTime = millis();
  
  int buttonsPressed = 0;

  clock_setTimer(gameTime);
  clock_start(CLOCK_MODE_TIMER);
  
  while((millis() - startTime) < gameTime){

    //Generate random button
    int buttonNumber = random(1, 65);

    //Turn on led
    led_set(buttonNumber, LED_STATE_ON);

    //Wait for press then turn of led
    button_wait(buttonNumber);
    led_set(buttonNumber, LED_STATE_OFF);

    //Increment counter
    buttonsPressed ++;
  }

  clock_stop();

  return buttonsPressed;
}

void game_countDown(){
  screen_display("3");
  delay(1000);
  screen_display("2");
  delay(1000);
  screen_display("1");
  delay(1000);
}

void game_start(){
  game_countDown();
  screen_display(String(game_runMain(30000)) + " buttons pressed.");

  
}

#endif
