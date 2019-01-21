#include "game.h"


Game::Game(ButtonReceiver *buttonInterface, const LedController &led) : buttonInterface(buttonInterface),
                                                                        ledManager(led) {}

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

    buttonInterface->addListener(this);

    startTime = millis();

    ledNumber = static_cast<uint8_t>(random(0, NUMBER_OF_LEDS));  //Generate random button
    ledManager.turnOn(ledNumber);
    ledManager.shiftOut();

    while (not isDone) {
        //screen::display(String(millis() - startTime));
        threadManager::runThreader();
    }

    unsigned long averageReactionSpeed = (millis() - startTime) / buttonsPressed;

    screen::display(String(averageReactionSpeed) + " average speed in millis");
    delay(2000);
}

void Game::buttonPressed(const uint8_t &buttonPressed) {
    if (buttonPressed == ledNumber) {
        buttonsPressed++;
        ledManager.turnOff(ledNumber);

        if (millis() > startTime + GAME_TIME) {
            buttonInterface->removeListener();
            ledManager.shiftOut();
            isDone = true;
        } else {
            ledNumber = static_cast<uint8_t>(random(0, NUMBER_OF_LEDS));  //Generate random button
            ledManager.turnOn(ledNumber);
            ledManager.shiftOut();
        }
    }
}