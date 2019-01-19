#include "lib/game.h"
#include "lib/bluetooth.h"
#include "lib/led-manager.h"
#include "lib/logger.h"
#include "lib/screen.h"
#include "lib/flasher.h"
#include "lib/constants.h"

void startReadyMode(Flasher flasher, ButtonManager buttonManager) {
    buttonManager.clearLast();
    screen::display("READY");
    flasher.startFlashing(0);
}

// Makes chasing lights on the outer circle
void bootUpSequence(LedManager led) {
    for (uint8_t i = 32; i < 64; i++) {
        led.turnOn(i);
        led.shiftOut();
        delay(70);
        led.turnOff(i);
    }

    for (uint8_t i = 0; i < 64; i++) {
        led.turnOff(i);
    }
    led.shiftOut();
}

void setup() {
    Serial.begin(9600);

    //Generate new random seed such that button sequence is different each time
    randomSeed(static_cast<unsigned long>(analogRead(0)));

    //Setup
    LedManager led; //Create an led manager
    Flasher flasher(led); //Create a flasher
    ButtonManager buttonManager = ButtonManager::get(); //TODO Take into account is_button_connected
    Bluetooth *bluetooth = nullptr;

    if (IS_BUTTONS_CONNECTED) {
        buttonManager.setup();
    }

    if (IS_BLUETOOTH_CHIP_CONNECTED) {
        Bluetooth bluetooth_tmp = Bluetooth(led, buttonManager);
        bluetooth = &bluetooth_tmp;
    }

    log(TYPE_INFO, "main", "Setup done");

    //bootUpSequence(led);

    if (not bluetooth or (not bluetooth->shouldGoOnline())) {
        startReadyMode(flasher, buttonManager); //Unless already connected start ready mode
    }

    while (true) {
        //If connected to bluetooth go in online mode
        if (bluetooth and bluetooth->shouldGoOnline()) {
            flasher.stopFlashing(0);
            screen::display("ONLINE");

            bluetooth->listen();

            //When disconnected
            startReadyMode(flasher, buttonManager);
        }

        //If middle button pressed go in offline mode
        if (buttonManager.isPressed(0)) {
            flasher.stopFlashing(0);

            Game(buttonManager, led).start();

            //When game ends
            startReadyMode(flasher, buttonManager);
        }

        flasher.checkFlash(); // Will flash if should flash
    }
}

void loop() {}