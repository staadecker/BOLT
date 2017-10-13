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
    unsigned long startTime;
    const unsigned long GAME_TIME = 3000;
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
      while (millis() - startTime < GAME_TIME) {

        //Generate random button
        int buttonNumber = random(0, constants::NUMBER_OF_LEDS - 1);

        //Turn on led and wait for button press
        led::turnOn(buttonNumber);
        
        while(not button::isPressed(buttonNumber)){
          controller::run();
        }
        
        led::turnOff(buttonNumber);

        //Increment counter
        buttonsPressed ++;
      }
    }
  }

  unsigned long getRemainingTime() {
    unsigned long remainingTime = GAME_TIME - (millis() - startTime);

    if (remainingTime > 0) {
      return remainingTime;
    }
    
    return 0;
  }

  void start() {
    countDown();
    
    startTime = millis();

    timer::start();
    runMain();
    timer::stop();

    screen::display(String(buttonsPressed) + " buttons pressed");
  }
}
