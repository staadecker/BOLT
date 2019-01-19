#include "lib/led.h"
#include "lib/constants.h"
#include "lib/button-manager.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

void cycleLights() {
    led::setup();

    unsigned long DELAY = 1000;

    uint8_t i = 0;
    while (true) {
        led::turnOn(i);
        led::shiftOut();
        delay(DELAY);
        led::turnOff(i);
        i++;
        if (i == NUMBER_OF_LEDS) {
            i = 0;
        }
    }
}

// To test light marked 3-4 on board use shiftRegister = 3 and value = 4
void singleLight(uint8_t shiftRegister, uint8_t value) {
    led::setup();
    led::turnOn(uint8_t(8) * (shiftRegister - uint8_t(1)) + (value - uint8_t(1)));
    led::shiftOut();
}

void allLightsTest() {
    led::setup();
    for (uint8_t i = 0; i < 64; i++) {
        led::turnOn(i);
    }
    led::shiftOut();
}

void printButtonPressTest() {
    ButtonManager::setup();
    int buttonLastPressed = ButtonManager::get().getButtonLastPressed();
    while (true) {
        if (ButtonManager::get().getButtonLastPressed() != buttonLastPressed) {
            buttonLastPressed = ButtonManager::get().getButtonLastPressed();
            Serial.println("Pressed button: " + String(buttonLastPressed));
        }
        delay(10);
    }
}

void buttonWithLEDTest() {
    ButtonManager::setup();
    led::setup();

    uint8_t i = 0;

    while (true) {
        ButtonManager::get().clearLast();
        led::turnOn(i);
        led::shiftOut();
        while (not ButtonManager::get().isPressed(i)) {
        }

        led::turnOff(i);
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