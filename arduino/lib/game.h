#ifndef GAME_H
#define GAME_H

#include "button-manager.h"

class Game {
    static const unsigned long GAME_TIME = 30000;
    unsigned long startTime;

    unsigned long getRemainingTime();

    ButtonManager buttonManager;

    void countDown();

    unsigned long runMain();

public:
    explicit Game(ButtonManager buttonManager);

    void start();
};

#endif
