//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_BUTTONDEBUG_H
#define BOLT_BUTTONDEBUG_H


#include "button-interface.h"
#include "threader.h"

class ButtonDebug : public ButtonInterface, public ThreaderCallback {
    ButtonCallbackInterface *listener = nullptr;
    uint8_t threaderId;

public:
    void setCallback(ButtonCallbackInterface *buttonCallbackInterface) override;

    void removeCallback() override;

    void call() override;
};


#endif //BOLT_BUTTONDEBUG_H
