#include "game.h"


Game::Game(ButtonReceiver *buttonReceiver, LedController &ledController, DoneGameCallback *doneGameCallback)
        : buttonReceiver(buttonReceiver), ledController(ledController), doneGameCallback(doneGameCallback) {}

void Game::start() {
    screen::display("3");
    delay(1000);
    screen::display("2");
    delay(1000);
    screen::display("1");
    delay(1000);

    buttonReceiver->addListener(this);

    startTime = millis();

    ledNumber = static_cast<unsigned char>(random(0, NUMBER_OF_LEDS));  //Generate random button
    ledController.turnOn(ledNumber);
    ledController.shiftOut();
}

void Game::buttonPressed(const unsigned char buttonPressed) {
    if (buttonPressed == ledNumber) {
        buttonsPressed++;
        ledController.turnOff(ledNumber);

        if (millis() <= startTime + GAME_TIME) {
            ledNumber = static_cast<unsigned char>(random(0, NUMBER_OF_LEDS));  //Generate random button
            ledController.turnOn(ledNumber);
            ledController.shiftOut();
        } else {
            buttonReceiver->removeListener();
            ledController.shiftOut();
            unsigned long averageReactionSpeed = (millis() - startTime) / buttonsPressed;
            char screenMessage[29];

            sprintf(screenMessage, "%04d", static_cast<int>(averageReactionSpeed));
            strcat(screenMessage, " average speed in millis");

            screen::display(screenMessage);
            delay(2000);
            doneGameCallback->goToStartMode();
        }
    }
}