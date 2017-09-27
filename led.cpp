#include "led.h"
#include "flasher.h"

//Array keeping track of states
uint8_t states[NUMBER_OF_BITS];

void led_shiftOut() {
  //Latch Low. VCC high
  digitalWrite(P_LED_VCC, HIGH);
  digitalWrite(P_LED_LATCH, LOW);


  //Shift out
  for (uint8_t i = NUMBER_OF_BITS; i <= NUMBER_OF_BITS; i--)  {
    switch (states[i]) {
      case LED_STATE_OFF:
        digitalWrite(P_LED_DATA, LOW);
        break;
      case LED_STATE_ON:
        digitalWrite(P_LED_DATA, HIGH);
        break;
      case LED_STATE_FLASHING:
        flashed = true;
        digitalWrite(P_LED_DATA, flashingCurrentState);
        break;
    }


    digitalWrite(P_LED_CLOCK, HIGH);
    digitalWrite(P_LED_CLOCK, LOW);
  }

  //If data is left on high drop to low, so that the final output voltage is not altered
  digitalWrite(P_LED_DATA, LOW);

  //Latch high. VCC adjust to two volts
  digitalWrite(P_LED_LATCH, HIGH);
  analogWrite(P_LED_VCC, LED_VCC_PWM);


}

void led_setState(uint8_t led, uint8_t state) {

  //If we have a flashing led turn on. The thread will automatically turn off if no leds are flashing.
  if (state == LED_STATE_FLASHING) {
    flasherThread.enabled = true;
  }

  states[led] = state;
}
