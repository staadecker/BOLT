#include "main.h"
#include "lib/buttonShieldReceiver.h"
#include "lib/buttonSerialReceiver.h"
#include "lib/screen.h"
#include "lib/game.h"


void setup() {
    MainRun().run();
}

void loop() {}


void MainRun::run() {
    Serial.begin(9600);

    //Generate new random seed such that button sequence is different each time
    randomSeed(static_cast<unsigned long>(analogRead(0)));

    //Create button shield
    if (IS_BUTTONS_CONNECTED) {
        ButtonShieldReceiver buttonManagerTemp = ButtonShieldReceiver::create(); //Create object
        buttonInterface = &buttonManagerTemp; //Assign to buttonInterface
    } else {
        ButtonSerialReceiver buttonDebugTemp; //Create object
        buttonInterface = &buttonDebugTemp; //Assign to buttonInterface
    }

    //Create bluetooth
    if (IS_BLUETOOTH_CHIP_CONNECTED) {
        Bluetooth bluetooth_tmp = Bluetooth(ledManager, buttonInterface); //Create object
        bluetooth = &bluetooth_tmp; //Assign to buttonInterface
    }

    if (IS_LED_CONNECTED) {
        bootUpSequence();
    }

    if (not bluetooth or (not bluetooth->shouldGoOnline())) {
        startReadyMode(); //Unless already connected start ready mode
    }

    while (true) {
        threadManager::runThreader();

        //If connected to bluetooth go in online mode
        if (bluetooth and bluetooth->shouldGoOnline()) {
            flasher.stopFlashing(0);
            screen::display("ONLINE");

            bluetooth->listen();

            //When disconnected
            startReadyMode();
        }
    }
}

// Makes chasing lights on the outer circle
void MainRun::bootUpSequence() {
    for (uint8_t i = 32; i < 64; i++) {
        ledManager.turnOn(i);
        ledManager.shiftOut();
        delay(70);
        ledManager.turnOff(i);
    }

    for (uint8_t i = 0; i < 64; i++) {
        ledManager.turnOff(i);
    }
    ledManager.shiftOut();
}

void MainRun::startReadyMode() {
    screen::display("READY");
    buttonInterface->addListener(this);
    flasher.startFlashing(0);
}

void MainRun::buttonPressed(const uint8_t &buttonNumber) {
    if (buttonNumber == 0) {
        flasher.stopFlashing(0);
        buttonInterface->removeListener();

        Game(buttonInterface, ledManager).start();

        //When game ends
        startReadyMode();
    }
}


