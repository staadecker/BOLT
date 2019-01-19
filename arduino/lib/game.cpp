#include "game.h"

#include "screen.h"
#include "led.h"
#include "timer.h"
#include "constants.h"

Game::Game(const ButtonManager buttonsManagerArg) : buttonManager(buttonsManagerArg) {}

void Game::countDown() {
    screen::display("3");
    delay(1000);
    screen::display("2");
    delay(1000);
    screen::display("1");
    delay(1000);
}

unsigned long Game::runMain() {

    uint8_t buttonsPressed = 0;

    while (millis() < startTime + GAME_TIME) {

        auto buttonNumber = static_cast<uint8_t>(random(0, NUMBER_OF_LEDS));  //Generate random button

        buttonManager.clearLast();
        led::turnOn(buttonNumber);  //Turn on led and wait for button press
        led::shiftOut();

        while (not buttonManager.isPressed(buttonNumber) and millis() < startTime + GAME_TIME) {
            timer::checkUpdateDisplay();
        }

        led::turnOff(buttonNumber);

        buttonsPressed++; //Increment counter
    }

    led::shiftOut();
    return (millis() - startTime) / buttonsPressed;
}

unsigned long Game::getRemainingTime() {
    unsigned long remainingTime = startTime + GAME_TIME - millis();

    if (remainingTime > 0) {
        return remainingTime;
    }

    return 0;
}

void Game::start() {
    countDown();

    startTime = millis();

    unsigned long averageReactionSpeed = runMain();

    screen::display(String(averageReactionSpeed) + " average speed in millis");
    delay(2000);

}
