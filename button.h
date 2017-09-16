/* Manages interaction with the buttons */
#ifndef BUTTON_H
#define BUTTON_H

const unsigned char BUTTON_NONE = 100;

bool button_isPressed(int buttonNumber);
void button_wait(int buttonNumber);
void button_ISR();

#endif
