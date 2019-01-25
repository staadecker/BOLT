//
// Created by machs on 2019-01-19.
//

#ifndef BOLT_MAIN_H
#define BOLT_MAIN_H

#include "lib/ledController.h"
#include "lib/flasher.h"
#include "lib/bluetoothManager.h"
#include "lib/buttonReceiver.h"
#include "lib/returnToStartingStateCallback.h"
#include "lib/offlineGame.h"

class StartingState : public ButtonPressListener, public ReturnToStartingStateCallback, public Runnable {
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

    void returnToStartState() override;

    void onRun() override;
};


#endif //BOLT_MAIN_H
