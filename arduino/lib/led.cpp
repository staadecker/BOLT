#include "led.h"
#include "flasher.h"
#include "constants.h"
#include "logger.h"
#include <Arduino.h>

namespace led {
namespace {
const uint8_t VCC_PWM = 95; //Value to adjust for optimal 2V output voltage

//Array keeping track of states
        bool states[NUMBER_OF_LEDS];
}

void shiftOut() {
  //Latch Low. VCC high
  digitalWrite(P_LED_VCC, HIGH);
  digitalWrite(P_LED_LATCH, LOW);


  //Shift out
  for (uint8_t i = NUMBER_OF_LEDS; i <= NUMBER_OF_LEDS; i--) {
    digitalWrite(P_LED_DATA, states[i]);

    //Clock
    digitalWrite(P_LED_CLOCK, HIGH);
    digitalWrite(P_LED_CLOCK, LOW);
  }

  //If data is left on high drop to low, so that the final output voltage is not altered
  digitalWrite(P_LED_DATA, LOW);

  //Latch high. VCC adjust to two volts
  digitalWrite(P_LED_LATCH, HIGH);
  analogWrite(P_LED_VCC, VCC_PWM);

}

void turnOn(uint8_t ledNumber) {
  logger::log(logger::TYPE_DEBUG, "led", "Set led number " + String(ledNumber) + " ON");

  states[ledNumber] = HIGH;
}

void turnOff(uint8_t ledNumber) {
  logger::log(logger::TYPE_DEBUG, "led", "Set led number " + String(ledNumber) + " OFF");

  states[ledNumber] = LOW;
}

void setup() {
  pinMode(P_LED_VCC, OUTPUT);
  pinMode(P_LED_DATA, OUTPUT);
  pinMode(P_LED_CLOCK, OUTPUT);
  pinMode(P_LED_LATCH, OUTPUT);
}
}
