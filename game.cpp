#include "game.h"

#include "button.h"
#include "screen.h"
#include "led.h"
#include "timer.h"
#include "const.h"
#include "logger.h"
#include "helper.h"
#include "controller.h"

namespace game {
  namespace {
    const unsigned long GAME_TIME = 3000;
    
    unsigned long endTime;
    
    unsigned int buttonsPressed = 0;

    void countDown() {
      screen::display("3");
      helper::waitTime(1000);
      screen::display("2");
      helper::waitTime(1000);
      screen::display("1");
      helper::waitTime(1000);
    }

    void runMain() {
      while (millis() < endTime) {

        //Generate random button
        int buttonNumber = random(0, constants::NUMBER_OF_LEDS - 1);

        //Turn on led and wait for button press
        led::turnOn(buttonNumber);
        
        while(not button::isPressed(buttonNumber) and millis() < endTime){
          controller::run();
        }

        if (millis() < endTime){
          buttonsPressed ++; //Increment counter
        }
        
        led::turnOff(buttonNumber);
      }
    }
  }

  unsigned long getRemainingTime() {
    unsigned long remainingTime = endTime - millis();

    if (remainingTime > 0) {
      return remainingTime;
    }
    
    return 0;
  }

  void start() {
    countDown();
    
    endTime = GAME_TIME + millis();

    timer::start();
    runMain();
    timer::stop();

    screen::display(String(buttonsPressed) + " buttons pressed");
  }
}
