#ifndef GAME_H
#define GAME_H

#include "buttonShieldReceiver.h"
#include "ledController.h"
#include "screen.h"
#include "threader.h"
#include "../main.h"
#include <USBAPI.h>

class Game : public ButtonPressListener {
    static const unsigned long GAME_TIME = 30000;
    unsigned long startTime;
    uint8_t buttonsPressed = 0;
    uint8_t ledNumber = 0;

    ButtonReceiver *buttonReceiver;
    LedController &ledController;
    DoneGameCallback *doneGameCallback;

    void countDown();

    void buttonPressed(const uint8_t &buttonPressed) override;

public:
    Game(ButtonReceiver *buttonReceiver, LedController &ledController, DoneGameCallback *doneGameCallback);

    void start();
};

#endif
