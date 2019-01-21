//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_MAIN_H
#define BOLT_MAIN_H


#include "lib/ledController.h"
#include "lib/flasher.h"
#include "lib/bluetooth.h"
#include "lib/buttonReceiver.h"

class MainRun : public ButtonPressListener {
    LedController ledManager; //Create an led manager
    Flasher flasher = Flasher(ledManager); //Create a flasher
    ButtonReceiver *buttonInterface = nullptr;
    Bluetooth *bluetooth = nullptr;

    void bootUpSequence();

    void startReadyMode();

    void buttonPressed(const uint8_t &buttonNumber) override;

public:
    void run();
};


#endif //BOLT_MAIN_H
