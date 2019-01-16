/* Manages interaction with the buttons */
#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

namespace button {
void buttonPressedCallback(int8_t buttonNumber);
bool isPressed(uint8_t buttonToCheck);
void clearLast();
void setup();
int getButtonLastPressed();
}

#endif
