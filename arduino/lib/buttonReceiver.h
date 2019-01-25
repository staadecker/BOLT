//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_BUTTONINTERFACE_H
#define BOLT_BUTTONINTERFACE_H


class ButtonPressListener {
public:
    virtual void onButtonPressed(unsigned char buttonPressed) = 0;
};

class ButtonPressReceiver {
protected:
    ButtonPressListener *buttonPressListener = nullptr;
public:
    void addListener(ButtonPressListener *buttonPressListener1);

    void removeListener();
};

#endif //BOLT_BUTTONINTERFACE_H
