//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_BUTTONDEBUG_H
#define BOLT_BUTTONDEBUG_H


#include "buttonReceiver.h"

class ButtonSerialReceiver : public ButtonReceiver {
public:
    void checkForButtonPress() override;
};


#endif //BOLT_BUTTONDEBUG_H
