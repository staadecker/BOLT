#include "lib/buttonShieldReceiver.h"
#include "lib/buttonSerialReceiver.h"
#include "lib/screen.h"
#include "lib/game.h"
#include "lib/bluetooth.h"
#include <MemoryUsage.h>
#include <Arduino.h>

LedController ledController; //Create an led manager
//Flasher flasher = Flasher(ledController); //Create a flasher
ButtonReceiver *buttonReceiver;
Bluetooth *bluetooth = nullptr;

class MainButtonListener : public ButtonPressListener {
    void buttonPressed(const unsigned char &buttonNumber) override {
        if (buttonNumber == 0) {
            //flasher.stopFlashing(0);
            buttonReceiver->removeListener();

            Game(buttonReceiver, ledController).start();

            screen::display("READY");
            buttonReceiver->addListener(this);

        }
    }
};

MainButtonListener mainButtonListener = MainButtonListener();

//// Makes chasing lights on the outer circle
static void bootUpSequence() {
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


void setup() {
    Serial.begin(9600);
    Serial.println(F("Starting code..."));

    //Generate new random seed such that button sequence is different each time
    randomSeed(static_cast<unsigned long>(analogRead(0)));

    //Create button shield
    if (IS_BUTTONS_CONNECTED) {
        buttonReceiver = &ButtonShieldReceiver::create(); //Assign to buttonInterface
    } else {
        static ButtonSerialReceiver buttonSerialReceiverTmp; //Create object
        buttonReceiver = &buttonSerialReceiverTmp; //Assign to buttonInterface
    }

    //Create bluetooth
    if (IS_BLUETOOTH_CHIP_CONNECTED) {
        static Bluetooth bluetooth_tmp = Bluetooth(ledController, buttonReceiver); //Create object
        bluetooth = &bluetooth_tmp; //Assign to buttonInterface
    }

    if (IS_LED_CONNECTED) {
        bootUpSequence();
    }

    screen::display("READY");
    buttonReceiver->addListener(&mainButtonListener);
}

void loop() {
    buttonReceiver->checkForButtonPress();

    if (bluetooth != nullptr and bluetooth->shouldGoOnline()) {
        //flasher.stopFlashing(0);
        buttonReceiver->removeListener();

        screen::display("ONLINE");
        bluetooth->goOnline();
        screen::display("READY");
        buttonReceiver->addListener(&mainButtonListener);
    }
}
