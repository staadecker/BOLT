//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_BUTTONINTERFACE_H
#define BOLT_BUTTONINTERFACE_H

#include <stdint.h>


class ButtonPressListener {
public:
    virtual void buttonPressed(const uint8_t &buttonPressed);
};

class ButtonReceiver {
protected:
    ButtonPressListener *listener = nullptr;
public:
    void addListener(ButtonPressListener *buttonPressListener);

    void removeListener();
};

#endif //BOLT_BUTTONINTERFACE_H
