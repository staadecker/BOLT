#ifndef GAME_H
#define GAME_H

#include "buttonShieldReceiver.h"
#include "ledController.h"
#include "screen.h"
#include "threader.h"
#include <USBAPI.h>

class Game : public ButtonPressListener {
    static const unsigned long GAME_TIME = 30000;
    unsigned long startTime;
    uint8_t buttonsPressed = 0;
    uint8_t ledNumber = 0;
    bool isDone = false;

    ButtonReceiver *buttonInterface;
    LedController ledManager;

    void countDown();

    void buttonPressed(const uint8_t &buttonPressed) override;

public:
    Game(ButtonReceiver *buttonInterface, const LedController &led);

    void start();
};

#endif
