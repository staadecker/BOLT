/* Manages interaction with the buttons */
#ifndef BUTTON_H
#define BUTTON_H

#include "button-interface.h"


class ButtonManager : public ButtonInterface {
    ButtonCallbackInterface *callback;

    ButtonManager(); //private constructor. Use get.

    static ButtonManager buttonManager;

    static void isr();

public:
    static ButtonManager create();

    void setCallback(ButtonCallbackInterface *callbackArg) override;

    void removeCallback() override;
};


#endif
