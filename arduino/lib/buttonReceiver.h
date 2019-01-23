//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_BUTTONINTERFACE_H
#define BOLT_BUTTONINTERFACE_H

#include <stdint.h>
#include <HardwareSerial.h>


class ButtonPressListener {
public:
    virtual void buttonPressed(const unsigned char &buttonPressed) = 0;
};

class ButtonReceiver {
protected:
    ButtonPressListener *listener = nullptr;
public:
    void addListener(ButtonPressListener *buttonPressListener);

    void removeListener();

    virtual void checkForButtonPress() = 0; //Used by buttonSerialReceiver
};

#endif //BOLT_BUTTONINTERFACE_H
