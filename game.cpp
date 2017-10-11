#include "game.h"

#include "button.h"
#include "screen.h"
#include "led.h"
#include "timer.h"
#include "const.h"
#include "logger.h"
#include "helper.h"

namespace game {
  namespace {
    unsigned long startTime;
    unsigned long totalTime;
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
      while (millis() - startTime < totalTime) {

        //Generate random button
        int buttonNumber = random(0, constants::NUMBER_OF_LEDS - 1);

        //Turn on led and wait for button press
        led::setState(buttonNumber, led::STATE_ON);
        button::wait(buttonNumber);
        led::setState(buttonNumber, led::STATE_OFF);

        //Increment counter
        buttonsPressed ++;
      }
    }
  }

  unsigned long getRemainingTime() {
    unsigned long remainingTime = totalTime - (millis() - startTime);

    if (remainingTime > 0) {
      return remainingTime;
    }

    return 0;
  }

  unsigned long getElapsedTime() {
    return millis() - startTime;
  }

  void start() {
    countDown();

    totalTime = 30000;
    startTime = millis();

    timer::start(timer::MODE_TIMER);
    runMain();
    timer::stop();

    screen::display(String(buttonsPressed) + " buttons pressed");
  }
}
