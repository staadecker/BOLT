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
#include "lib/game.h"

class MainRun : public ButtonPressListener, public DoneGameCallback, public Thread {
    LedController ledController; //Create an led manager
    Flasher flasher = Flasher(ledController); //Create a flasher
    ButtonReceiver *buttonReceiver = nullptr;
    Bluetooth *bluetooth = nullptr;
    Game *game = nullptr;

    void bootUpSequence();

    void buttonPressed(unsigned char buttonNumber) override;

    void exitStartMode();

public:
    void setup();

    void goToStartMode() override;

    void runThread() override;
};


#endif //BOLT_MAIN_H
