/* Manages interaction with the buttons */
#ifndef BUTTON_H
#define BUTTON_H

#include "buttonReceiver.h"


class ButtonShieldReceiver : public ButtonReceiver {
    ButtonShieldReceiver(); //private constructor. Use static method create() to make instance.

    static ButtonShieldReceiver buttonShield;

    static void isr();

public:
    static ButtonShieldReceiver create();
};


#endif
