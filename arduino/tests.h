#ifndef TESTS_H
#define TESTS_H

namespace tests {
void cycleLights();

// To test light marked 3-4 on board use shiftRegister = 3 and value = 4
void singleLight(int shiftRegister, int value);

void printButtonPress();
void testButtonWithLED();
void allLightsTest();
}

#endif
