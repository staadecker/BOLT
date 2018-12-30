#include "led.h"
#include "flasher.h"
#include "constants.h"
#include "logger.h"
#include <Arduino.h>

namespace led {
namespace {
const uint8_t VCC_PWM = 95; //Value to adjust for optimal 2V output voltage

//Array keeping track of states
bool states[constants::NUMBER_OF_LEDS];
}

void shiftOut() {
  //Latch Low. VCC high
  digitalWrite(constants::P_LED_VCC, HIGH);
  digitalWrite(constants::P_LED_LATCH, LOW);


  //Shift out
  for (uint8_t i = constants::NUMBER_OF_LEDS; i <= constants::NUMBER_OF_LEDS; i--)  {
    digitalWrite(constants::P_LED_DATA, states[i]);

    //Clock
    digitalWrite(constants::P_LED_CLOCK, HIGH);
    digitalWrite(constants::P_LED_CLOCK, LOW);
  }

  //If data is left on high drop to low, so that the final output voltage is not altered
  digitalWrite(constants::P_LED_DATA, LOW);

  //Latch high. VCC adjust to two volts
  digitalWrite(constants::P_LED_LATCH, HIGH);
  analogWrite(constants::P_LED_VCC, VCC_PWM);

}

void turnOn(uint8_t ledNumber) {
  logger::log(logger::TYPE_INFO, "led", "Set led number " + String(ledNumber) + " ON");

  states[ledNumber] = HIGH;
}

void turnOff(uint8_t ledNumber) {
  logger::log(logger::TYPE_INFO, "led", "Set led number " + String(ledNumber) + " OFF");

  states[ledNumber] = LOW;
}

void setup() {
  pinMode(constants::P_LED_VCC, OUTPUT);
  pinMode(constants::P_LED_DATA, OUTPUT);
  pinMode(constants::P_LED_CLOCK, OUTPUT);
  pinMode(constants::P_LED_LATCH, OUTPUT);
}
}
