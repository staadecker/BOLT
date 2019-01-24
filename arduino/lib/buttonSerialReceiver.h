//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_BUTTONDEBUG_H
#define BOLT_BUTTONDEBUG_H


#include "buttonReceiver.h"
#include "threader.h"

class ButtonSerialReceiver : public ButtonReceiver, public Thread {
public:
    ButtonSerialReceiver();

    void runThread() override;
};


#endif //BOLT_BUTTONDEBUG_H
