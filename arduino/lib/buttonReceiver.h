//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_BUTTONINTERFACE_H
#define BOLT_BUTTONINTERFACE_H


class ButtonPressListener {
public:
    virtual void buttonPressed(unsigned char buttonPressed) = 0;
};

class ButtonReceiver {
protected:
    ButtonPressListener *listener = nullptr;
public:
    void addListener(ButtonPressListener *buttonPressListener);

    void removeListener();
};

#endif //BOLT_BUTTONINTERFACE_H
