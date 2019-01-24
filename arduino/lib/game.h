#ifndef GAME_H
#define GAME_H

#include "buttonShieldReceiver.h"
#include "ledController.h"
#include "screen.h"
#include "doneGameCallback.h"
#include <USBAPI.h>

class Game : public ButtonPressListener {
    static const unsigned long GAME_TIME = 30000;
    unsigned long startTime;
    unsigned char buttonsPressed = 0;
    unsigned char ledNumber = 0;

    ButtonReceiver *buttonReceiver;
    LedController &ledController;
    DoneGameCallback *doneGameCallback;

public:
    Game(ButtonReceiver *buttonReceiver, LedController &ledController, DoneGameCallback *doneGameCallback);

    void start();

    void buttonPressed(unsigned char buttonPressed) override;
};

#endif
