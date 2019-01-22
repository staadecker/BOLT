#include "main.h"
#include "lib/buttonShieldReceiver.h"
#include "lib/buttonSerialReceiver.h"
#include "lib/screen.h"
#include "lib/game.h"


void setup() {
    MainRun().run();
}

void loop() {
    threadManager::runThreader();
}

MainRun::MainRun() = default;

void MainRun::run() {
    Serial.begin(9600);

    //Generate new random seed such that button sequence is different each time
    randomSeed(static_cast<unsigned long>(analogRead(0)));

    //Create button shield
    if (IS_BUTTONS_CONNECTED) {
        ButtonShieldReceiver buttonManagerTemp = ButtonShieldReceiver::create(); //Create object
        buttonReceiver = &buttonManagerTemp; //Assign to buttonInterface
    } else {
        ButtonSerialReceiver buttonDebugTemp; //Create object
        buttonReceiver = &buttonDebugTemp; //Assign to buttonInterface
    }

    //Create bluetooth
    if (IS_BLUETOOTH_CHIP_CONNECTED) {
        Bluetooth bluetooth_tmp = Bluetooth(ledController, buttonReceiver, this); //Create object
        bluetooth = &bluetooth_tmp; //Assign to buttonInterface
    }

    if (IS_LED_CONNECTED) {
        bootUpSequence();
    }

    done(); // Go to the same mode as if a game ended
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

void MainRun::buttonPressed(const uint8_t &buttonNumber) {
    if (buttonNumber == 0) {
        flasher.stopFlashing(0);
        buttonReceiver->removeListener();
        threadManager::removeThread(this);

        Game(buttonReceiver, ledController, this).start();
    }
}

void MainRun::done() {
    screen::display("READY");
    buttonReceiver->addListener(this);
    flasher.startFlashing(0);
    threadManager::addThread(this);
}

void MainRun::runThread() {
    if (bluetooth and bluetooth->shouldGoOnline()) {
        flasher.stopFlashing(0);
        buttonReceiver->removeListener();
        threadManager::removeThread(this);

        screen::display("ONLINE");
        bluetooth->goOnline();
    }
}

