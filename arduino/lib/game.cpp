#include "game.h"

#include "button.h"
#include "screen.h"
#include "led.h"
#include "timer.h"
#include "constants.h"
#include "logger.h"

namespace game {
  namespace {
    const unsigned long GAME_TIME = 30000;
    
    unsigned long startTime;

    void countDown() {
      screen::display("3");
      delay(1000);
      screen::display("2");
      delay(1000);
      screen::display("1");
      delay(1000);
    }

    unsigned long runMain() {

      uint8_t buttonsPressed = 0;
      
      while (millis() < startTime + GAME_TIME) {

          int buttonNumber = random(0, NUMBER_OF_LEDS);  //Generate random button

        button::clearLast();
        led::turnOn(buttonNumber);  //Turn on led and wait for button press
        led::shiftOut();
        
        while(not button::isPressed(buttonNumber) and millis() < startTime + GAME_TIME){
          timer::checkUpdateDisplay();
        }

        led::turnOff(buttonNumber);
        
        if (millis() < startTime + GAME_TIME){
          buttonsPressed ++; //Increment counter
        } else {
          led::shiftOut();
          return (millis() - startTime)/buttonsPressed;
        }
        
        
      }
    }
  }

  unsigned long getRemainingTime() {
    unsigned long remainingTime = startTime + GAME_TIME - millis();

    if (remainingTime > 0) {
      return remainingTime;
    }
    
    return 0;
  }

  void start() {
    countDown();
    
    startTime = millis();

    unsigned long averageReactionSpeed = runMain();

    screen::display(String(averageReactionSpeed) + " average speed in millis");
    delay(2000);

  }
}
