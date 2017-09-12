/*   This class manages the LEDs.  
 *   
 *   Use led_set to set the led to one of the following states.
 *   
*/
#ifndef LED
#define LED

bit led_states[64];




const int LED_TWO_VOLTS = 195;

const int LED_STATE_OFF = 0;
const int LED_STATE_ON = 1;
const int LED_STATE_FLASHING = 2;


void led_set(int ledNumber, int state){
  Serial.println("LED number " + String(ledNumber) + " set to state " + String(state));

  if (state == LED_STATE_FLASHING){
    Serial.println("I do not want to make this function!!!!");
    //TODO
  }
  else{
    led_states(ledNumber-1) = state;
  }

  
  
  
  //TODO
}

void led_shiftOut(uint8_t bitOrder, uint8_t val)
{
    for (uint8_t i = 0; i < 8; i++)  {
        if (!!(val & (1 << (7 - i)))){
          analogWrite(P_LED_DATA, LED_TWO_VOLTS);
        } else{
          analogWrite(P_LED_DATA, 0);
        }

        analogWrite(P_LED_CLOCK, LED_TWO_VOLTS);
        analogWrite(P_LED_CLOCK, 0);        
    }
}

void led_setup(){
  pinMode(P_LED_VCC, OUTPUT);
  pinMode(P_LED_DATA, OUTPUT);
  pinMode(P_LED_CLOCK, OUTPUT);
  pinMode(P_LED_LATCH, OUTPUT);
  
  analogWrite(P_LED_VCC, LED_TWO_VOLTS);

  for (uint8_t i = 0; i < 64; i++) {
    led_states[i] = 0;
  }
}

#endif
