#include <USBAPI.h>
#include "offlineGame.h"


OfflineGame::OfflineGame(ButtonPressReceiver *buttonReceiver, LedController &ledController,
                         ReturnToStartingStateCallback *returnToStartingStateCallback)
        : buttonPressReceiver(buttonReceiver), ledController(ledController),
          returnToStartingStateCallback(returnToStartingStateCallback) {}

void OfflineGame::startGame() {
    screen::displayOnScreen("3...");
    delay(1000);
    screen::displayOnScreen("2...");
    delay(1000);
    screen::displayOnScreen("1...");
    delay(1000);

    buttonPressReceiver->addListener(this);

    gameStartTime = millis();

    currentLedTurnedOn = static_cast<unsigned char>(random(0, NUMBER_OF_LEDS));  //Generate random button
    ledController.turnOnLed(currentLedTurnedOn);
    ledController.shiftOutLEDs();
}

void OfflineGame::onButtonPressed(const unsigned char buttonPressed) {
    if (buttonPressed == currentLedTurnedOn) {
        buttonsPressedInGame++;
        ledController.turnOffLed(currentLedTurnedOn);

        if (millis() < gameStartTime + GAME_DURATION) {
            currentLedTurnedOn = static_cast<unsigned char>(random(0, NUMBER_OF_LEDS));  //Generate new random button
            ledController.turnOnLed(currentLedTurnedOn);
            ledController.shiftOutLEDs();
        } else {
            //GAME ENDED
            buttonPressReceiver->removeListener();
            ledController.shiftOutLEDs();

            unsigned long averageReactionSpeed = (millis() - gameStartTime) / buttonsPressedInGame;

            char screenMessage[29];
            sprintf(screenMessage, "%04d", static_cast<int>(averageReactionSpeed));
            strcat(screenMessage, " average speed in millis");

            screen::displayOnScreen(screenMessage);
            delay(2000);

            returnToStartingStateCallback->returnToStartState();
        }
    }
}