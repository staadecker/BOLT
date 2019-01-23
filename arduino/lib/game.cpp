#include "game.h"


Game::Game(ButtonReceiver *buttonReceiver, LedController &ledController)
        : buttonReceiver(buttonReceiver), ledController(ledController) {}

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

    buttonReceiver->addListener(this);

    startTime = millis();

    ledNumber = static_cast<uint8_t>(random(0, NUMBER_OF_LEDS));  //Generate random button
    ledController.turnOn(ledNumber);
    ledController.shiftOut();

    while (isRunning) {
        buttonReceiver->checkForButtonPress();
    }
}

void Game::buttonPressed(const uint8_t &buttonPressed) {
    if (buttonPressed == ledNumber) {
        buttonsPressed++;
        ledController.turnOff(ledNumber);

        if (millis() <= startTime + GAME_TIME) {
            ledNumber = static_cast<uint8_t>(random(0, NUMBER_OF_LEDS));  //Generate random button
            ledController.turnOn(ledNumber);
            ledController.shiftOut();
        } else {
            buttonReceiver->removeListener();
            ledController.shiftOut();
            unsigned long averageReactionSpeed = (millis() - startTime) / buttonsPressed;
            char screenMessage[29];

            sprintf(screenMessage, "%04d", averageReactionSpeed);
            strcat(screenMessage, " average speed in millis");

            screen::display(screenMessage);
            delay(2000);
        }
    }
}