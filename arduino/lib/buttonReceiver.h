//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_BUTTONINTERFACE_H
#define BOLT_BUTTONINTERFACE_H


#include "runnablesManager.h"

class ButtonPressListener {
public:
    virtual void onButtonPressed(unsigned char buttonPressed) = 0;
};

class ButtonPressReceiver : public Runnable {
public:
    ButtonPressListener *buttonPressListener = nullptr;
    void addListener(ButtonPressListener *buttonPressListener1);

    void removeListener();
};

#endif //BOLT_BUTTONINTERFACE_H
