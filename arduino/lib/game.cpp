#include <USBAPI.h>
#include "game.h"

#include "screen.h"
#include "threader.h"

Game::Game(ButtonInterface *buttonInterfaceArg, LedManager ledArg) : buttonInterface(buttonInterfaceArg),
                                                                     ledManager(ledArg) {}

void Game::countDown() {
    screen::display("3");
    delay(1000);
    screen::display("2");
    delay(1000);
    screen::display("1");
    delay(1000);
}

void Game::start() {
    countDown();

    buttonInterface->setCallback(this);

    startTime = millis();

    ledNumber = static_cast<uint8_t>(random(0, NUMBER_OF_LEDS));  //Generate random button
    ledManager.turnOn(ledNumber);
    ledManager.shiftOut();

    while (not isDone) {
        //screen::display(String(millis() - startTime));
        threader::runThreader();
    }

    unsigned long averageReactionSpeed = (millis() - startTime) / buttonsPressed;

    screen::display(String(averageReactionSpeed) + " average speed in millis");
    delay(2000);
}

void Game::call(uint8_t buttonPressed) {
    if (buttonPressed == ledNumber) {
        buttonsPressed++;
        ledManager.turnOff(ledNumber);

        if (millis() > startTime + GAME_TIME) {
            buttonInterface->removeCallback();
            ledManager.shiftOut();
            isDone = true;
        } else {
            ledNumber = static_cast<uint8_t>(random(0, NUMBER_OF_LEDS));  //Generate random button
            ledManager.turnOn(ledNumber);
            ledManager.shiftOut();
        }
    }
}