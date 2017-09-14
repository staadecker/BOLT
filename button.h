/* Manages interaction with the buttons */
#ifndef BUTTON
#define BUTTON

#include "const.h"
#include "controller.h"

const uint8_t BUTTON_NONE = 100;

//Stores the value of the button pressed or depressed
volatile uint8_t button_pressed = BUTTON_NONE;

//Called when the P_BUTTON_INTERRUPT is falling
void button_ISR() {

  uint8_t button = 0;

  for (int i = 0; i < 8; i++) {

    //Wait for clock to go high
    while (digitalRead(P_BUTTON_CLOCK) == LOW);

    //Read and add value to button
    if (digitalRead(P_BUTTON_DATA) == HIGH)
    {
      button = button + 1;
    }

    //If not last time in loop, shift bits
    if (i != 7) {
      button = button << 1;
    }

    //Wait for clock to go low
    while (digitalRead(P_BUTTON_CLOCK) == HIGH);
  }

  if (button > 64) {
    button_pressed = button - 129;
  }

}

void button_isPressed(int buttonNumber) {
  if (button_pressed == buttonNumber) {
    button_pressed = BUTTON_NONE ;
    return true;
  }
  return false;
}

//Ends when the button with buttonNumber is pressed
void button_wait(int buttonNumber) {
  while (not button_isPressed) {
    controller_run();
  }
}

#endif
