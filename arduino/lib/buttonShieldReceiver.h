/* Manages interaction with the buttons */
#ifndef BUTTON_H
#define BUTTON_H

#include "buttonReceiver.h"
#include "runnablesManager.h"

class ButtonShieldButtonPressReceiver : public ButtonPressReceiver, public Runnable {
    ButtonShieldButtonPressReceiver(); //private constructor. Use static method create() to make instance.

    static ButtonShieldButtonPressReceiver instanceOfButtonShield;

    static void isrReadButtonPress();

    static unsigned char latestButton;
    static bool interuptCalled;

public:
    void onRun() override;

    static ButtonShieldButtonPressReceiver &create();
};


#endif
