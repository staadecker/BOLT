#include "main.h"
#include "tests.h"

void setup() {
  Serial.begin(9600);

  //Decide what you want to run.
  main::runMain();
  
  //tests::cycleLights();
  //tests::singleLights();
  //tests::printButtonPress();
  //tests::testButtonWithLED();
  //tests::allLightsTest();
}

//Loop not used since we just use a while loop in runMain();
void loop() {}
