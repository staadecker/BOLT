#ifndef GAME_H
#define GAME_H

#include "buttonShieldReceiver.h"
#include "ledController.h"
#include "screen.h"
#include <USBAPI.h>

class Game : public ButtonPressListener {
    static const unsigned long GAME_TIME = 30000;
    unsigned long startTime;
    unsigned char buttonsPressed = 0;
    unsigned char ledNumber = 0;
    bool isRunning = true;

    ButtonReceiver *buttonReceiver;
    LedController &ledController;

    void countDown();

    void buttonPressed(const uint8_t &buttonPressed) override;

public:
    Game(ButtonReceiver *buttonReceiver, LedController &ledController);

    void start();
};

#endif
