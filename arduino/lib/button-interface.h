//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_BUTTONINTERFACE_H
#define BOLT_BUTTONINTERFACE_H

class ButtonCallbackInterface {
public:
    virtual void call(uint8_t buttonPressed);
};

class ButtonInterface {
public:
    virtual void setCallback(ButtonCallbackInterface *buttonCallbackInterface);

    virtual void removeCallback();
};

#endif //BOLT_BUTTONINTERFACE_H
