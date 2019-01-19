/* Manages interaction with the buttons */
#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class ButtonManager {
    //Stores the value of the button pressed
    volatile bool buttonWasPressed = false;
    volatile uint8_t buttonPressed = 0;

    ButtonManager();

    static ButtonManager buttonManager;

    static void isr();

public:
    static void setup();

    static ButtonManager get();

    void buttonPressedCallback(uint8_t buttonNumber);

    bool isPressed(int buttonToCheck);

    void clearLast();

    int8_t getButtonLastPressed();
};


#endif
