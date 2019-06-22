#ifndef GAME_H
#define GAME_H

#include "buttonShieldReceiver.h"
#include "ledController.h"
#include "returnToStartingStateCallback.h"

class OfflineGame : public ButtonPressListener {
    static const unsigned long GAME_DURATION = 30000;
    unsigned long gameStartTime;
    unsigned char buttonsPressedInGame;
    unsigned char currentLedTurnedOn = 0;

    ButtonPressReceiver *buttonPressReceiver;
    LedController &ledController;
    ReturnToStartStateCallback *returnToStartingStateCallback;

public:
    OfflineGame(ButtonPressReceiver *buttonReceiver, LedController &ledController,
                ReturnToStartStateCallback *returnToStartingStateCallback);

    void startGame();

    void onButtonPressed(unsigned char buttonPressed) override;
};

#endif
