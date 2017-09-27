//CONSTANTS
const uint8_t P_LED_VCC = 5;
const uint8_t P_LED_DATA = 8;
const uint8_t P_LED_CLOCK = 12;
const uint8_t P_LED_LATCH = 13;

//Value to adjust for optimal output voltage
const uint8_t LED_VCC_PWM = 105;

//0 is always default value
const uint8_t LED_STATE_OFF = 0;
const uint8_t LED_STATE_ON = 1;

const uint8_t NUMBER_OF_BITS = 16;

//PROCEDURE

//Array keeping track of states
uint8_t states[NUMBER_OF_BITS];

void setup() {
  Serial.begin(115200);

  // put your setup code here, to run once:
  pinMode(P_LED_VCC, OUTPUT);
  pinMode(P_LED_DATA, OUTPUT);
  pinMode(P_LED_CLOCK, OUTPUT);
  pinMode(P_LED_LATCH, OUTPUT);

  states[1] = LED_STATE_ON;
  states[4] = LED_STATE_ON;
  states[5] = LED_STATE_ON;
  states[8] = LED_STATE_ON;
  states[10] = LED_STATE_ON;
  shiftOut();
}

void loop() {}

void shiftOut() {
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

