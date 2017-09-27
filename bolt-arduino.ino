const int P_LED_VCC = 5;
const int P_LED_DATA = 8;
const int P_LED_CLOCK = 12;
const int P_LED_LATCH = 13;

//Value to adjust for optimal output voltage
const uint8_t LED_VCC_PWM = 105;

//const uint8_t LED_STATE_OFF = 0;
//const uint8_t LED_STATE_ON = 1;


byte val = B01000000;

//byte states[8];

void setup() {
  Serial.begin(115200);

  // put your setup code here, to run once:
  pinMode(P_LED_VCC, OUTPUT);
  pinMode(P_LED_DATA, OUTPUT);
  pinMode(P_LED_CLOCK, OUTPUT);
  pinMode(P_LED_LATCH, OUTPUT);

  shiftOut();
  delay(5000);

  val = B11111111;
  shiftOut();
}

void loop() {}


void shiftOut() {
  //Latch Low vcc high
  digitalWrite(P_LED_VCC, HIGH);
  digitalWrite(P_LED_LATCH, LOW);
  

  //Shift out
  for (uint8_t i = 0; i < 8; i++)  {
    if (!!(val & (1 << i))) {
      digitalWrite(P_LED_DATA, HIGH);
    } else {
      digitalWrite(P_LED_DATA, LOW);
    }


    digitalWrite(P_LED_CLOCK, HIGH);
    digitalWrite(P_LED_CLOCK, LOW);
  }

  //If data is left on high drop to low, so that the final voltage is not altered
  digitalWrite(P_LED_DATA, LOW);

  //Latch high, vcc adjust
  digitalWrite(P_LED_LATCH, HIGH);
  analogWrite(P_LED_VCC, LED_VCC_PWM);


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
}

