#ifndef GAME_H
#define GAME_H

#include "button-manager.h"
#include "led-manager.h"

class Game : public ButtonCallbackInterface {
    static const unsigned long GAME_TIME = 30000;
    unsigned long startTime;
    uint8_t buttonsPressed = 0;
    uint8_t ledNumber = 0;
    bool isDone = false;

    ButtonInterface *buttonInterface;
    LedManager ledManager;

    void countDown();

    void call(uint8_t buttonPressed) override;

public:
    Game(ButtonInterface *buttonInterface, LedManager led);

    void start();
};

#endif
