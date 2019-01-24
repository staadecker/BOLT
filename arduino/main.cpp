#include "main.h"
#include "lib/buttonSerialReceiver.h"

MainRun mainRun; //Must be define globally to still be accessible in loop function

void setup() {
    mainRun.setup();
}

void loop() {
    threadManager::runThreader(); //Parts of the code will add themselves to the threadManager who will run them.
}

void MainRun::setup() {
    Serial.begin(9600);
    Serial.println(F("Starting..."));

    //Generate new random seed (so that the button sequence is different each time)
    randomSeed(static_cast<unsigned long>(analogRead(0)));

    //Create button receiver.
    //If button shield is connected then use ButtonShieldReceiver.
    //Else receive button presses from Serial using ButtonSerialReceiver
    if (IS_BUTTONS_CONNECTED) {
        buttonReceiver = &ButtonShieldReceiver::create(); //Assign to buttonInterface
    } else {
        static ButtonSerialReceiver buttonSerialReceiver = ButtonSerialReceiver(); //Create object
        buttonReceiver = &buttonSerialReceiver; //Assign to buttonInterface
    }

    //If bluetooth chip is connected created bluetooth object
    if (IS_BLUETOOTH_CHIP_CONNECTED) {
        static Bluetooth bluetooth_tmp = Bluetooth(ledController, buttonReceiver, this); //Create object
        bluetooth = &bluetooth_tmp; //Assign to buttonInterface
    }

    //If LED's connected run boot sequence
    if (IS_LED_CONNECTED) {
        bootUpSequence();
    }

    goToStartMode();
}

void MainRun::goToStartMode() {
    screen::display("READY");
    buttonReceiver->addListener(this); // Register for callback to see if button 0 is pressed (indicating start of game)
    flasher.startFlashing(0); // Flash button zero to make user start game
    threadManager::addThread(this); //Add this as thread to constantly check if received start packet from bluetooth
}

void MainRun::exitStartMode() {
    buttonReceiver->removeListener();
    flasher.stopFlashing(0);
    threadManager::removeThread(this);
}

void MainRun::buttonPressed(const unsigned char buttonPressed) {
    if (buttonPressed == 0) {
        exitStartMode();

        if (game == nullptr) {
            //Create a game object
            static Game gameTmp = Game(buttonReceiver, ledController, this);
            game = &gameTmp;
        }

        game->start();
    }
}


void MainRun::runThread() {
    if (bluetooth and bluetooth->shouldGoOnline()) {
        exitStartMode();
        screen::display("ONLINE");

        bluetooth->goOnline();
    }
}

// Makes chasing lights on the outer circle
void MainRun::bootUpSequence() {
    for (uint8_t i = 32; i < 64; i++) {
        ledController.turnOn(i);
        ledController.shiftOut();
        delay(70);
        ledController.turnOff(i);
    }

    for (uint8_t i = 0; i < 64; i++) {
        ledController.turnOff(i);
    }
    ledController.shiftOut();
}