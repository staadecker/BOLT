#ifndef LED
#define LED

void setLedState(int ledNumber, int state){
  Serial.println("LED number " + String(ledNumber) + " set to state " + String(state));
  //TODO
}
#endif
