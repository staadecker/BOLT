#include "tests.h"
#include "led.h"
#include "constants.h"
#include "button.h"

namespace tests {

void cycleLights() {
  led::setup();

  int DELAY = 1000;

  int i = 0;
  while (true) {
    led::turnOn(i);
    led::shiftOut();
    delay(DELAY);
    led::turnOff(i);
    i++;
    if (i == constants::NUMBER_OF_LEDS) {
      i = 0;
    }
  }
}

// To test light marked 3-4 on board use shiftRegister = 3 and value = 4
void singleLight(int shiftRegister, int value) {
  led::setup();
  led::turnOn(8 * (shiftRegister - 1) + (value - 1));
  led::shiftOut();
}

void printButtonPress() {
  button::setup();
  int buttonLastPressed = button::getButtonLastPressed();
  while (true) {
    if (button::getButtonLastPressed() != buttonLastPressed) {
      buttonLastPressed = button::getButtonLastPressed();
      Serial.println("Pressed button: " + String(buttonLastPressed));
    }
    delay(10);
  }
}

void testButtonWithLED() {
  button::setup();
  led::setup();

  int i = 0;

  while (true) {
    button::clearLast();
    led::turnOn(i);
    led::shiftOut();
    while (not button::isPressed(i)) {
    }

    led::turnOff(i);
    i++;
    if (i == constants::NUMBER_OF_LEDS) {
      i = 0;
    }
  }
}
}
