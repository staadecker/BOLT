#include "main.h"
#include "tests.h"

void setup() {
  Serial.begin(9600);

  //Decide what you want to run.
  //main::runMain() for the main program or tests for a specific test
  //main::runMain();
  //tests::cycleLights();
  //tests::singleLights();
  tests::printButtonPress();
}

//Loop not used since we just use a while loop in runMain();
void loop() {}
