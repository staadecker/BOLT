#include "constants.h"
#include "led.h"


void setup() {
  Serial.begin(9600);

  // put your setup code here, to run once:
  led::setup();

  // testSingleLight();
  cycleLights();
}

void loop() {
}

void cycleLights() {
  int DELAY = 1000;

  int i = 0;
  while (true) {
    led::turnOn(i);
    led::shiftOut();
    delay(DELAY);
    led::turnOff(i);
    i++;
    if (i == constants::NUMBER_OF_LEDS) {
      i = 0;
    }
  }
}

// To test light marked 3-4 on board use shiftRegister = 3 and value = 4
void testSingleLight() {
  int shiftRegister = 8;
  int value = 8;
  led::turnOn(8 * (shiftRegister - 1) + (value - 1));

  led::shiftOut();
}
