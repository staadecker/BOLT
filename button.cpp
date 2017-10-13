#include "button.h"

#include "const.h"
#include "controller.h"
#include "logger.h"
#include "helper.h"

namespace button {
  namespace {
    //Stores the value of the button pressed or depressed
    const uint8_t BUTTON_NONE = 65;
    volatile uint8_t buttonPressed = BUTTON_NONE;

    //Method to call when the the interrupt pin is falling
    void isr() {

      buttonPressed = 0;

      for (int i = 0; i < 8; i++) {

        //Wait for clock to go high
        while (digitalRead(constants::P_BUTTON_CLOCK) == LOW);

        //If value is high add one
        buttonPressed = buttonPressed + digitalRead(constants::P_BUTTON_DATA);

        //If not last time in loop, shift bits
        if (i != 7) {
          buttonPressed = buttonPressed << 1;
        }

        //Wait for clock to go low
        while (digitalRead(constants::P_BUTTON_CLOCK) == HIGH);
      }

      if (buttonPressed > 64) {
        buttonPressed = buttonPressed - 129;
      }
    }



  }

  //Checks whether the button got pressed
  bool isPressed(uint8_t buttonToCheck) {
    //If not button shield wait two seconds and return true
    if (not constants::IS_BUTTON_SHIELD_CONNECTED) {
      helper::waitTime(2000);
      return true;
    }

    //If button pressed is the button to check return true
    if (buttonPressed == buttonToCheck) {
      buttonPressed = BUTTON_NONE;
      return true;
    }
    return false;
  }

  //Ends when buttonToWaitFor is pressed
  void wait(uint8_t buttonToWaitFor) {
    logger::log(logger::TYPE_INFO, "button", "Waiting for press on button : " + String(buttonToWaitFor));

    while (not isPressed(buttonToWaitFor) ) {
      controller::run();
    }
  }

  void setup() {
    attachInterrupt(digitalPinToInterrupt(constants::P_BUTTON_INTERRUPT), isr, FALLING); //Attach interrupt for 64 button shield
  }
}
