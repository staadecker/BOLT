//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_MAIN_H
#define BOLT_MAIN_H

#include "lib/ledController.h"
#include "lib/flasher.h"
#include "lib/bluetooth.h"
#include "lib/buttonReceiver.h"
#include "lib/returnToReadyModeCallback.h"
#include "lib/game.h"

class ReadyMode : public ButtonPressListener, public ReturnToReadyModeCallback, public Runnable {
    LedController ledController; //Create an led manager
    Flasher flasher = Flasher(ledController); //Create a flasher
    ButtonPressReceiver *buttonReceiver = nullptr;
    BluetoothManager *bluetoothManager = nullptr;
    OfflineGame *offlineGame = nullptr;

    void bootUpSequence();

    void onButtonPressed(unsigned char buttonNumber) override;

    void exitReadyMode();

public:
    void setup();

    void returnToReadyMode() override;

    void onRun() override;
};


#endif //BOLT_MAIN_H
