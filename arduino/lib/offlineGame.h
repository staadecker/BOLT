#ifndef GAME_H
#define GAME_H

#include "buttonShieldReceiver.h"
#include "ledController.h"
#include "screen.h"
#include "returnToStartingStateCallback.h"
#include <USBAPI.h>

class OfflineGame : public ButtonPressListener {
    static const unsigned long GAME_DURATION = 30000;
    unsigned long gameStartTime;
    unsigned char buttonsPressedInGame = 0;
    unsigned char currentLedTurnedOn = 0;

    ButtonPressReceiver *buttonPressReceiver;
    LedController &ledController;
    ReturnToStartingStateCallback *returnToStartingStateCallback;

public:
    OfflineGame(ButtonPressReceiver *buttonReceiver, LedController &ledController,
                ReturnToStartingStateCallback *returnToStartingStateCallback);

    void startGame();

    void onButtonPressed(unsigned char buttonPressed) override;
};

#endif
