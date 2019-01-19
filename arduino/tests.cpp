#include "lib/led-manager.h"
#include "lib/constants.h"
#include "lib/button-manager.h"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

void cycleLights() {
    Serial.begin(9600);
    LedManager ledManager;

    unsigned long DELAY = 1000;

    uint8_t i = 0;
    while (true) {
        ledManager.turnOn(i);
        ledManager.shiftOut();
        delay(DELAY);
        ledManager.turnOff(i);
        i++;
        if (i == NUMBER_OF_LEDS) {
            i = 0;
        }
    }
}

// To test light marked 3-4 on board use shiftRegister = 3 and value = 4
void singleLight(uint8_t shiftRegister, uint8_t value) {
    Serial.begin(9600);
    LedManager ledManager;

    ledManager.turnOn(uint8_t(8) * (shiftRegister - uint8_t(1)) + (value - uint8_t(1)));
    ledManager.shiftOut();
}

void allLightsTest() {
    Serial.begin(9600);
    LedManager ledManager;

    for (uint8_t i = 0; i < 64; i++) {
        ledManager.turnOn(i);
    }
    ledManager.shiftOut();
}

void printButtonPressTest() {
    Serial.begin(9600);

    ButtonManager buttonManager = ButtonManager::get();

    buttonManager.setup();
    int buttonLastPressed = buttonManager.getButtonLastPressed();
    while (true) {
        if (buttonManager.getButtonLastPressed() != buttonLastPressed) {
            buttonLastPressed = buttonManager.getButtonLastPressed();
            Serial.println("Pressed button: " + String(buttonLastPressed));
        }
        delay(10);
    }
}

void buttonWithLEDTest() {
    Serial.begin(9600);
    LedManager ledManager;
    ButtonManager buttonManager = ButtonManager::get();

    buttonManager.setup();

    uint8_t i = 0;

    while (true) {
        buttonManager.clearLast();
        ledManager.turnOn(i);
        ledManager.shiftOut();
        while (not buttonManager.isPressed(i)) {
        }

        ledManager.turnOff(i);
        i++;
        if (i == NUMBER_OF_LEDS) {
            i = 0;
        }
    }
}


void setup() {
    //cycleLights();
    //singleLight(3, 4);
    //allLightsTest();
    //printButtonPressTest();
    //buttonWithLEDTest();
}

void loop() {}


#pragma clang diagnostic pop