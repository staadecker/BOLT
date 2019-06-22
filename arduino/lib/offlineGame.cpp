#include <USBAPI.h>
#include "offlineGame.h"


OfflineGame::OfflineGame(ButtonPressReceiver *buttonReceiver, LedController &ledController,
                         ReturnToStartStateCallback *returnToStartingStateCallback)
        : buttonPressReceiver(buttonReceiver), ledController(ledController),
          returnToStartingStateCallback(returnToStartingStateCallback) {}

void OfflineGame::startGame() {
    delay(3000);

    buttonPressReceiver->addListener(this);

    gameStartTime = millis();
    buttonsPressedInGame = 0;

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

            char resultMessage[29];
            sprintf(resultMessage, "%04d", static_cast<int>(averageReactionSpeed));
            strcat(resultMessage, " average speed in millis");
            Serial.println(resultMessage);

            delay(3000);

            returnToStartingStateCallback->returnToStartState();
        }
    }
}