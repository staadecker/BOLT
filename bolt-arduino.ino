const int P_LED_VCC = 5;
const int P_LED_DATA = 9;
const int P_LED_CLOCK = 10;
const int P_LED_LATCH = 11;

const uint8_t LED_TWO_VOLTS = 255;

//const uint8_t LED_STATE_OFF = 0;
//const uint8_t LED_STATE_ON = 1;


byte val = B01010110;

//byte states[8];

void setup() {
  Serial.begin(115200);

  // put your setup code here, to run once:
  pinMode(P_LED_VCC, OUTPUT);
  pinMode(P_LED_DATA, OUTPUT);
  pinMode(P_LED_CLOCK, OUTPUT);
  pinMode(P_LED_LATCH, OUTPUT);

  analogWrite(P_LED_VCC, LED_TWO_VOLTS);

  delay(1000);

  //states[0] = LED_STATE_ON;

  digitalWrite(P_LED_LATCH, LOW);
  shiftOut();
  digitalWrite(P_LED_LATCH, HIGH);

}

void loop() {}


void shiftOut() {
  for (uint8_t i = 0; i < 8; i++)  {
    if (!!(val & (1 << i))){
      digitalWrite(P_LED_DATA, HIGH);
    } else {
      digitalWrite(P_LED_DATA, LOW);
    }
    

    digitalWrite(P_LED_CLOCK, HIGH);
    digitalWrite(P_LED_CLOCK, LOW);
  }

  /*analogWrite(P_LED_CLOCK, LOW);

    //Loop for every byte
    for (uint8_t i = 0; i < 8; i++)  {
    Serial.println("Index : " + String(i) + ". State : OFF " + String(!!(val & (1 << i))));
    digitalWrite(P_LED_DATA, !!(val & (1 << i)));
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
    analogWrite(P_LED_CLOCK, HIGH);
    analogWrite(P_LED_CLOCK, LOW);
    }
  */


  Serial.println("Done");
}

