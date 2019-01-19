//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_MAIN_H
#define BOLT_MAIN_H

#include "lib/button-interface.h"
#include "lib/flasher.h"


class MainRun : public ButtonCallbackInterface {
    LedManager ledManager; //Create an led manager
    Flasher flasher = Flasher(ledManager); //Create a flasher
    ButtonInterface *buttonInterface = nullptr;
    Bluetooth *bluetooth = nullptr;

    void bootUpSequence();

    void startReadyMode(Flasher flasher, ButtonInterface *buttonInterface);

    void call(uint8_t buttonNumber) override;

public:
    void run();
};


#endif //BOLT_MAIN_H
