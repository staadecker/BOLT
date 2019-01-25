#include "main.h"
#include "lib/buttonSerialReceiver.h"

StartingState startingState; //Must be define globally to still be accessible in loop function

void setup() {
    startingState.setup();
}

void loop() {
    runnablesManager::execute(); //Parts of the code will add themselves to the runnablesManager who will run them.
}

void StartingState::setup() {
    Serial.begin(9600);
    Serial.println(F("Starting..."));

    //Generate new random seed (so that the button sequence is different each time)
    randomSeed(static_cast<unsigned long>(analogRead(0)));

    //Create button receiver.
    //If button shield is connected then use ButtonShieldReceiver.
    //Else receive button presses from Serial using ButtonSerialReceiver
    if (IS_BUTTONS_CONNECTED) {
        buttonReceiver = &ButtonShieldButtonPressReceiver::create(); //Assign to buttonInterface
    } else {
        static SerialButtonPressReceiver buttonSerialReceiver = SerialButtonPressReceiver(); //Create object
        buttonReceiver = &buttonSerialReceiver; //Assign to buttonInterface
    }

    //If bluetooth chip is connected created bluetooth object
    if (IS_BLUETOOTH_CHIP_CONNECTED) {
        static BluetoothManager bluetooth_tmp = BluetoothManager(ledController, buttonReceiver, this); //Create object
        bluetoothManager = &bluetooth_tmp; //Assign to buttonInterface
    }

    //If LED's connected run boot sequence
    if (IS_LED_CONNECTED) {
        bootUpSequence();
    }

    returnToStartState(); //More setup (same method as when a game ends and you are returning to the starting state)
}

void StartingState::returnToStartState() {
    screen::displayOnScreen("READY");
    buttonReceiver->addListener(this); // Register for callback to see if button 0 is pressed (indicating start of game)
    flasher.startFlashingLED(0); // Flash button zero to make user start game
    runnablesManager::addRunnable(
            this); //Add this as thread to constantly check if received start packet from bluetooth
}

void StartingState::exitReadyMode() {
    buttonReceiver->removeListener();
    flasher.stopFlashingLED(0);
    runnablesManager::removeRunnable(this);
}

void StartingState::onButtonPressed(const unsigned char buttonPressed) {
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


void StartingState::onRun() {
    if (bluetoothManager and bluetoothManager->shouldGoInBluetoothState()) {
        exitReadyMode();
        screen::displayOnScreen("ONLINE");

        bluetoothManager->goInBluetoothState();
    }
}

// Makes chasing lights on the outer circle
void StartingState::bootUpSequence() {
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