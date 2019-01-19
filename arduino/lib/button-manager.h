/* Manages interaction with the buttons */
#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class ButtonCallbackInterface {
public:
    virtual void call(uint8_t buttonPressed);
};

class ButtonManager {
    //Stores the value of the button pressed
    volatile bool buttonWasPressed = false;
    volatile uint8_t buttonPressed = 0;

    ButtonCallbackInterface *callback;

    ButtonManager(); //private constructor. Use get.

    static ButtonManager buttonManager;

    static void staticIsr();

    void isr();

public:
    void setup();

    static ButtonManager get();


    bool isPressed(int buttonToCheck);

    void clearLast();

    void setCallback(ButtonCallbackInterface *callbackArg);

    int8_t getButtonLastPressed();
};


#endif
