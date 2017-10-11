#include "button.h"

#include "const.h"
#include "controller.h"
#include "logger.h"
#include "helper.h"

namespace button {

  namespace {
    //Stores the value of the button pressed or depressed
    const unsigned char BUTTON_NONE = 100;
    volatile uint8_t button_pressed = BUTTON_NONE;
  }

  //Called when the P_BUTTON_INTERRUPT is falling
  void isr() {
    uint8_t button = 0;

    for (int i = 0; i < 8; i++) {

      //Wait for clock to go high
      while (digitalRead(constants::P_BUTTON_CLOCK) == LOW);

      //Read and add value to button
      if (digitalRead(constants::P_BUTTON_DATA) == HIGH)
      {
        button = button + 1;
      }

      //If not last time in loop, shift bits
      if (i != 7) {
        button = button << 1;
      }

      //Wait for clock to go low
      while (digitalRead(constants::P_BUTTON_CLOCK) == HIGH);
    }

    if (button > 64) {
      button_pressed = button - 129;
    }

  }

  bool isButtonPressed(uint8_t buttonNumber) {
    if (button_pressed == buttonNumber) {
      button_pressed = BUTTON_NONE ;
      return true;
    }
    return false;
  }

  //Ends when the button with buttonNumber is pressed
  void wait(uint8_t buttonNumber) {

    logger::logger(logger::TYPE_INFO, "button", "Waiting for press on button : " + String(buttonNumber));

    //If no board wait 2 seconds instead
    if (constants::IS_BUTTON_SHIELD_CONNECTED) {
      while (not isButtonPressed(buttonNumber) ) {
        controller::runController();
      }
    } else {
      helper::waitTime(2000);
    }
  }
}
