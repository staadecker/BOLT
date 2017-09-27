const int P_LED_VCC = 3;
const int P_LED_DATA = 9;
const int P_LED_CLOCK = 10;
const int P_LED_LATCH = 11;

const int LED_TWO_VOLTS = 105;

const uint8_t LED_STATE_OFF = 0;
const uint8_t LED_STATE_ON = 1;


//byte val = B10101010;

byte states[8];

void setup() {
  Serial.begin(115200);
  
  // put your setup code here, to run once:
  pinMode(P_LED_VCC, OUTPUT);
  pinMode(P_LED_DATA, OUTPUT);
  pinMode(P_LED_CLOCK, OUTPUT);
  pinMode(P_LED_LATCH, OUTPUT);

  analogWrite(P_LED_VCC, LED_TWO_VOLTS);

  states[0] = LED_STATE_ON;

  shiftOut();

}

void loop() {}


void shiftOut() {

  analogWrite(P_LED_LATCH, LOW);

  //Loop for every byte
  for (uint8_t i = 0; i < 8; i++)  {
    //Write byte
    
    switch (states[i]) {
      case LED_STATE_OFF:
      Serial.println("Index : " + String(i) + ". State : OFF " + String(states[i]));
        analogWrite(P_LED_DATA, LOW);
        break;
      case LED_STATE_ON:
      Serial.println("Index : " + String(i) + ". State : ON " + String(states[i]));
        analogWrite(P_LED_DATA, LED_TWO_VOLTS);
        break;
    }

    //Clock
    analogWrite(P_LED_CLOCK, LED_TWO_VOLTS);
    analogWrite(P_LED_CLOCK, LOW);
  }

  analogWrite(P_LED_LATCH, LED_TWO_VOLTS);
}

