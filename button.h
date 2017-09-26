/* Manages interaction with the buttons */
#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

const unsigned char BUTTON_NONE = 100;

bool button_isPressed(uint8_t buttonNumber);
void button_wait(uint8_t buttonNumber);
void button_ISR();

#endif
