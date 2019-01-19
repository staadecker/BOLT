#include "lib/game.h"
#include "lib/bluetooth.h"
#include "lib/led.h"
#include "lib/logger.h"
#include "lib/screen.h"
#include "lib/flasher.h"
#include "lib/constants.h"

void startReadyMode() {
    ButtonManager::get().clearLast();
    screen::display("READY");
    flasher::startFlashing(0);
}

// Makes chasing lights on the outer circle
void bootUpSequence() {
    for (uint8_t i = 32; i < 64; i++) {
        led::turnOn(i);
        led::shiftOut();
        delay(70);
        led::turnOff(i);
    }

    for (uint8_t i = 0; i < 64; i++) {
        led::turnOff(i);
    }
    led::shiftOut();
}

void setup() {
    //Generate new random seed such that button sequence is different each time
    randomSeed(static_cast<unsigned long>(analogRead(0)));

    //Setup
    bluetooth::setup();

    if (IS_BUTTONS_CONNECTED) {
        ButtonManager::setup();
    }

    led::setup();
    logger::log(logger::TYPE_INFO, "main", "Setup done");

    bootUpSequence();

    if (not bluetooth::shouldGoOnline()) {
        startReadyMode(); //Unless already connected start ready mode
    }
}

void loop() {
    //If connected to bluetooth go in online mode
    if (bluetooth::shouldGoOnline()) {
        flasher::stopFlashing(0);
        screen::display("ONLINE");

        bluetooth::listen();

        //When disconnected
        startReadyMode();
    }

    //If middle button pressed go in offline mode
    if (ButtonManager::get().isPressed(0)) {
        flasher::stopFlashing(0);

        Game(ButtonManager::get()).start();

        //When game ends
        startReadyMode();
    }

    flasher::checkFlash(); // Will flash if should flash
}
