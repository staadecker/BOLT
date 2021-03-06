//
// Created by machs on 1/24/2019.
//

#ifndef BOLT_STARTSTATE_H
#define BOLT_STARTSTATE_H


#include "bluetoothManager.h"
#include "flasher.h"
#include "offlineGame.h"


class StartState : public ButtonPressListener, public ReturnToStartStateCallback
#if IS_BLUETOOTH_CHIP_CONNECTED
        , public Runnable
#endif
{
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

#if IS_BLUETOOTH_CHIP_CONNECTED

    void onRun() override;

#endif
};

#endif //BOLT_STARTSTATE_H
