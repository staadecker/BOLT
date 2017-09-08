/*   This class manages the LEDs.  
 *   
 *   Use led_set to set the led to one of the following states.
 *   
*/
#ifndef LED
#define LED

const int LED_STATE_OFF = 0;
const int LED_STATE_FLASHING = 1;
const int LED_STATE_ON = 2;

void led_set(int ledNumber, int state){
  Serial.println("LED number " + String(ledNumber) + " set to state " + String(state));
  //TODO
}
#endif
