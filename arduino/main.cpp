#include "lib/game.h"
#include "lib/bluetooth.h"
#include "lib/logger.h"
#include "lib/screen.h"
#include "lib/flasher.h"
#include "main.h"
#include "lib/button-debug.h"


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
        ButtonManager buttonManagerTemp = ButtonManager::create();
        buttonInterface = &buttonManagerTemp;
    } else {
        ButtonDebug buttonDebugTemp;
        buttonInterface = &buttonDebugTemp;
        buttonInterface->removeCallback();
    }

    //Create bluetooth
    if (IS_BLUETOOTH_CHIP_CONNECTED) {
        Bluetooth bluetooth_tmp = Bluetooth(ledManager, buttonInterface);
        bluetooth = &bluetooth_tmp;
    }

    if (IS_LED_CONNECTED) {
        bootUpSequence();
    }

    if (not bluetooth or (not bluetooth->shouldGoOnline())) {
        startReadyMode(flasher, buttonInterface); //Unless already connected start ready mode
    }

    while (true) {
        threader::runThreader();

        //If connected to bluetooth go in online mode
        if (bluetooth and bluetooth->shouldGoOnline()) {
            flasher.stopFlashing(0);
            screen::display("ONLINE");

            bluetooth->listen();

            //When disconnected
            startReadyMode(flasher, buttonInterface);
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

void MainRun::startReadyMode(Flasher flasher, ButtonInterface *buttonInterface) {
    screen::display("READY");
    buttonInterface->setCallback(this);
    flasher.startFlashing(0);
}

void MainRun::call(uint8_t buttonNumber) {
    if (buttonNumber == 0) {
        flasher.stopFlashing(0);
        buttonInterface->removeCallback();

        Game(buttonInterface, ledManager).start();

        //When game ends
        startReadyMode(flasher, buttonInterface);
    }
}


