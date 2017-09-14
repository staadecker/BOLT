/*   This class manages the LEDs.

     Use led_set to set the led to one of the following states.

*/
#ifndef LED
#define LED

//Analog write constant for approx. 2V
const int LED_TWO_VOLTS = 195;

//Led states
const int LED_STATE_OFF = 0;
const int LED_STATE_ON = 1;
const int LED_STATE_FLASHING = 2;

//Stores the led states
uint8_t led_states[64];

//Variables for flashing
boolean led_flashingOn = true;
uint8_t led_flashingCounter = 0;

void led_setup() {
  pinMode(P_LED_VCC, OUTPUT);
  pinMode(P_LED_DATA, OUTPUT);
  pinMode(P_LED_CLOCK, OUTPUT);
  pinMode(P_LED_LATCH, OUTPUT);

  analogWrite(P_LED_VCC, LED_TWO_VOLTS);

  //Populate led_states
  for (uint8_t i = 0; i < sizeof(led_states); i++) {
    led_states[i] = LED_STATE_OFF;
  }
}

void led_update() {
  analogWrite(P_LED_LATCH, LED_TWO_VOLTS);

  for (uint8_t i = 63; i >= 0; i--)  {
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
        Serial.println("Error : Wrong Led Status Code in variable led_states");
        break;
    }
  }

  analogWrite(P_LED_CLOCK, LED_TWO_VOLTS);
  analogWrite(P_LED_CLOCK, 0);

  analogWrite(P_LED_LATCH, 0);
}

void led_set(int ledNumber, int newState) {

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

#endif
