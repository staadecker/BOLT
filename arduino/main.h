//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_MAIN_H
#define BOLT_MAIN_H

#include "lib/ledController.h"
#include "lib/flasher.h"
#include "lib/bluetooth.h"
#include "lib/buttonReceiver.h"
#include "lib/doneGameCallback.h"

class MainRun : public ButtonPressListener, public DoneGameCallback, public Thread {
    LedController ledController; //Create an led manager
    Flasher flasher = Flasher(ledController); //Create a flasher
    ButtonReceiver *buttonReceiver = nullptr;
    Bluetooth *bluetooth = nullptr;

    void bootUpSequence();

    void buttonPressed(const uint8_t &buttonNumber) override;

public:
    MainRun();
    void run();

    void done() override;

    void runThread() override;
};


#endif //BOLT_MAIN_H
