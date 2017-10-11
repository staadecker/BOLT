#include "game.h"

#include "button.h"
#include "screen.h"
#include "led.h"
#include "timer.h"
#include "const.h"
#include "logger.h"

namespace game {
  namespace {
    unsigned long startTime;
    unsigned long totalTime;

    void countDown() {
      screen::displayToScreen("3");
      delay(1000);
      screen::displayToScreen("2");
      delay(1000);
      screen::displayToScreen("1");
      delay(1000);
    }

    int runMain(unsigned long gameTime) {

      int buttonsPressed = 0;

      totalTime = gameTime;
      startTime = millis();

      timer::start(timer::MODE_TIMER);

      while ((millis() - startTime) < gameTime) {

        //Generate random button
        int buttonNumber = random(0, constants::NUMBER_OF_LEDS - 1);

        //Turn on led
        led::setState(buttonNumber, led::STATE_ON);

        //Wait for press then turn of led
        button::wait(buttonNumber);
        led::setState(buttonNumber, led::STATE_OFF);

        //Increment counter
        buttonsPressed ++;
      }

      timer::stopTimer();

      return buttonsPressed;
    }
  }

  unsigned long getRemainingTime() {
    unsigned long remainingTime = totalTime - (millis() - startTime);
    logger::logger(logger::TYPE_DEBUG, "game", String(startTime) + "     " + String(totalTime) + "    " + String(remainingTime));
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
    screen::displayToScreen(String(runMain(30000)) + " buttons pressed");
  }
}
