#include "led.h"
#include "flasher.h"
#include "const.h"
#include "logger.h"
#include <Arduino.h>

namespace led {
  namespace {
    //Array keeping track of states
    uint8_t states[constants::NUMBER_OF_LEDS];
    uint8_t currentFlasherState = LOW;

    void shiftOutLed() {
      //Latch Low. VCC high
      digitalWrite(constants::P_LED_VCC, HIGH);
      digitalWrite(constants::P_LED_LATCH, LOW);


      //Shift out
      for (uint8_t i = constants::NUMBER_OF_LEDS; i <= constants::NUMBER_OF_LEDS; i--)  {
        switch (states[i]) {
          case STATE_OFF:
            digitalWrite(constants::P_LED_DATA, LOW);
            break;
          case STATE_ON:
            digitalWrite(constants::P_LED_DATA, HIGH);
            break;
          case STATE_FLASHING:
            digitalWrite(constants::P_LED_DATA, currentFlasherState);
            break;
        }

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
  }

  void flash() {
    currentFlasherState = !currentFlasherState;
    shiftOutLed();
  }

  void setState(uint8_t led, uint8_t state) {
    logger::log(logger::TYPE_INFO, "led", "Set led number " + String(led) + " to state " + String(state));

    states[led] = state;

    shiftOutLed();
  }

  void setup() {
    pinMode(constants::P_LED_VCC, OUTPUT);
    pinMode(constants::P_LED_DATA, OUTPUT);
    pinMode(constants::P_LED_CLOCK, OUTPUT);
    pinMode(constants::P_LED_LATCH, OUTPUT);
  }
}
