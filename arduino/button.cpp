#include "button.h"

#include "constants.h"
#include "logger.h"
#include "bluetooth.h"

namespace button {
namespace {
//Stores the value of the button pressed or depressed
const int8_t BUTTON_NONE = -1;
volatile int8_t buttonPressed = BUTTON_NONE;

//Method to call when the the interrupt pin is falling
void isr() {

  uint8_t button = 0;

  for (int i = 0; i < 8; i++) {

    while (digitalRead(constants::P_BUTTON_CLOCK) == LOW);  //Wait for clock to go high

    button = button + digitalRead(constants::P_BUTTON_DATA);  //If value is high add one

    //If not last time in loop, shift bits
    if (i != 7) {
      button = button << 1;
    }

    while (digitalRead(constants::P_BUTTON_CLOCK) == HIGH);  //Wait for clock to go low
  }

  if (button > 64) {  // If button pressed
    buttonPressedCallback(button - 129);
  }
}
}

void buttonPressedCallback(int8_t buttonNumber) {
  buttonPressed = buttonNumber;
  bluetooth::sendPacket(bluetooth::C_BUTTON_PRESS + String(buttonPressed));
}

//Checks whether the button got pressed
bool isPressed(uint8_t buttonToCheck) {
  //If not button shield wait two seconds and return true
  if (not constants::IS_BUTTONS_CONNECTED  and not constants::IS_DEBUGGING) {
    delay(2000);
    return true;
  }

  //If button pressed is the button to check return true
  if (buttonPressed == buttonToCheck) {
    return true;
  }
  return false;
}

void clearLast() {
  buttonPressed = BUTTON_NONE;
}

int getButtonLastPressed() {
  return buttonPressed;
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(constants::P_BUTTON_INTERRUPT), isr, FALLING); //Attach interrupt for 64 button shield
}
}
