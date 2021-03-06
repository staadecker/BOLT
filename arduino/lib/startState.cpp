//
// Created by machs on 1/24/2019.
//

#include <HardwareSerial.h>
#include <USBAPI.h>
#include "startState.h"
#include "buttonSerialReceiver.h"


void StartState::setup() {
    Serial.begin(9600);
    Serial.println(F("Starting..."));

    //Generate new random seed (so that the button sequence is different each time)
    randomSeed(static_cast<unsigned long>(analogRead(0)));


    //Create button receiver.
    //If button shield is connected then use ButtonShieldReceiver.
    //Else receive button presses from Serial using ButtonSerialReceiver
#if IS_BUTTONS_CONNECTED
    buttonReceiver = &ButtonShield::create(); //Assign to buttonInterface
#else
    static SerialButtonPressReceiver buttonSerialReceiver = SerialButtonPressReceiver(); //Create object
    buttonReceiver = &buttonSerialReceiver; //Assign to buttonInterface
#endif


#if IS_BLUETOOTH_CHIP_CONNECTED
    //If bluetooth chip is connected created bluetooth object
    static BluetoothManager bluetooth_tmp = BluetoothManager(ledController, buttonReceiver, this); //Create object
    bluetoothManager = &bluetooth_tmp; //Assign to buttonInterface
#endif


#if IS_LED_CONNECTED
    //If LED's connected run boot sequence
    bootUpSequence();
#endif

    returnToStartState(); //More setup (same method as when a game ends and you are returning to the starting state)
}

void StartState::returnToStartState() {
    screen::displayOnScreen("READY");
    buttonReceiver->addListener(this); // Register for callback to see if button 0 is pressed (indicating start of game)
    flasher.startFlashingLED(0); // Flash button zero to make user start game
#if IS_BLUETOOTH_CHIP_CONNECTED
    runnablesManager::addRunnable(
            this); //Add this as thread to constantly check if received start packet from bluetooth
#endif
}

void StartState::exitReadyMode() {
    buttonReceiver->removeListener();
    flasher.stopFlashingLED(0);
#if IS_BLUETOOTH_CHIP_CONNECTED
    runnablesManager::removeRunnable(this);
#endif
}

void StartState::onButtonPressed(const unsigned char buttonPressed) {
    if (buttonPressed == 0) {
        exitReadyMode();

        if (offlineGame == nullptr) {
            //Create a game object
            static OfflineGame gameTmp = OfflineGame(buttonReceiver, ledController, this);
            offlineGame = &gameTmp;
        }

        offlineGame->startGame();
    }
}

#if IS_BLUETOOTH_CHIP_CONNECTED

void StartState::onRun() {
    if (bluetoothManager and bluetoothManager->shouldGoInBluetoothState()) {
        exitReadyMode();
        screen::displayOnScreen("ONLINE");

        bluetoothManager->goInBluetoothState();
    }
}

#endif

// Makes chasing lights on the outer circle
void StartState::bootUpSequence() {
    for (unsigned char i = 32; i < 64; i++) {
        ledController.turnOnLed(i);
        ledController.shiftOutLEDs();
        delay(70);
        ledController.turnOffLed(i);
    }

    for (unsigned char i = 0; i < 64; i++) {
        ledController.turnOffLed(i);
    }
    ledController.shiftOutLEDs();
}