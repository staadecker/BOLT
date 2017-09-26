#include "led.h"

#include "const.h"
#include "logger.h"
#include <Arduino.h>

//Stores the led states
unsigned char led_states[64];

//Variables for flashing
bool led_flashingOn = true;

unsigned char led_flashingCounter = 0;

void led_setup() {
  pinMode(P_LED_VCC, OUTPUT);
  pinMode(P_LED_DATA, OUTPUT);
  pinMode(P_LED_CLOCK, OUTPUT);
  pinMode(P_LED_LATCH, OUTPUT);

  analogWrite(P_LED_VCC, LED_TWO_VOLTS);

  //Populate led_states
  for (int i = 0; i < sizeof(led_states); i++) {
    led_states[i] = LED_STATE_OFF;
  }
}

void led_update() {
  logger(LOGGER_TYPE_DEBUG, "led", "Shift out");
  analogWrite(P_LED_LATCH, LED_TWO_VOLTS);
  for (uint8_t i = 63; i < 64; i--)  {
    switch (led_states[i]) {
      case LED_STATE_ON :
        analogWrite(P_LED_DATA, LED_TWO_VOLTS);
        break;
      case LED_STATE_OFF :
        analogWrite(P_LED_DATA, 0);
        break;
      case LED_STATE_FLASHING :
        if (led_flashingOn) {
          analogWrite(P_LED_DATA, LED_TWO_VOLTS);
        }
        else {
          analogWrite(P_LED_DATA, 0);
        }
        led_flashingOn = ! led_flashingOn;
        break;
      default:
        logger(LOGGER_TYPE_ERROR, "led", "wrong Led Status Code in variable led_states");
        break;
    }
    
  }

  analogWrite(P_LED_CLOCK, LED_TWO_VOLTS);
  analogWrite(P_LED_CLOCK, 0);

  analogWrite(P_LED_LATCH, 0);
}

void led_set(int ledNumber, int newState) {
  logger(LOGGER_TYPE_INFO, "led", "Set led : " + String(ledNumber) + " to state " + String(newState));
  uint8_t previousState = led_states[ledNumber];
  
  //If led is set to flashing and it wasn't before add one
  if (newState == LED_STATE_FLASHING and previousState != LED_STATE_FLASHING) {
    led_flashingCounter += 1;
  }
  //If led is set to stop flashing minus one
  if (previousState == LED_STATE_FLASHING and newState != LED_STATE_FLASHING) {
    led_flashingCounter -= 1;
  }

  led_states[ledNumber] = newState;

  led_update();
}
