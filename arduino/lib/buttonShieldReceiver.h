/* Manages interaction with the buttons */
#ifndef BUTTON_H
#define BUTTON_H

#include "buttonReceiver.h"


class ButtonShieldButtonPressReceiver : public ButtonPressReceiver {
    ButtonShieldButtonPressReceiver(); //private constructor. Use static method create() to make instance.

    static ButtonShieldButtonPressReceiver instanceOfButtonShield;

    static void readButtonPress();

public:
    static ButtonShieldButtonPressReceiver &create();
};


#endif
