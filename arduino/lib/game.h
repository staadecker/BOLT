#ifndef GAME_H
#define GAME_H

#include "button-manager.h"
#include "led-manager.h"

class Game {
    static const unsigned long GAME_TIME = 30000;
    unsigned long startTime;

    ButtonManager buttonManager;
    LedManager led;

    void countDown();

    unsigned long runMain();

public:
    Game(ButtonManager buttonManager, LedManager led);

    void start();
};

#endif
