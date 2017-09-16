/*   This class manages the LEDs.

     Use led_set to set the led to one of the following states.

*/
#ifndef LED_H
#define LED_H

//Analog write constant for approx. 2V
const int LED_TWO_VOLTS = 195;

//Led states
const int LED_STATE_OFF = 0;
const int LED_STATE_ON = 1;
const int LED_STATE_FLASHING = 2;

extern unsigned char led_flashingCounter;

void led_setup();
void led_update();
void led_set(int ledNumber, int newState);

#endif
