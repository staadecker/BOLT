#include "game.h"

#include "button.h"
#include "screen.h"
#include "led.h"
#include "clock.h"
#include "const.h"

unsigned long game_startTime;
unsigned long game_totalTime;

unsigned long game_getRemainingTime(){
  unsigned long remainingTime = game_totalTime - (millis() - game_startTime);
  if (remainingTime > 0){
    return remainingTime;
  }
  return 0;
}

unsigned long game_getElapsedTime(){
  return millis() - game_startTime;
}

void game_countDown() {
  screen_display("3");
  delay(1000);
  screen_display("2");
  delay(1000);
  screen_display("1");
  delay(1000);
}

int game_runMain(unsigned long gameTime) {

  int buttonsPressed = 0;

  game_totalTime = gameTime;
  game_startTime = millis();
  
  clock_start(CLOCK_MODE_TIMER);

  while ((millis() - game_startTime) < gameTime) {

    //Generate random button
    int buttonNumber = random(0, 63);

    //Turn on led
    led_setState(buttonNumber, LED_STATE_ON);

    //Wait for press then turn of led
    button_wait(buttonNumber);
    led_setState(buttonNumber, LED_STATE_OFF);

    //Increment counter
    buttonsPressed ++;
  }

  clock_stop();

  return buttonsPressed;
}

void game_start() {
  game_countDown();
  screen_display(String(game_runMain(30000)) + " buttons pressed.");
}
