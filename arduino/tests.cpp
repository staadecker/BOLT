#include <HardwareSerial.h>
#include <USBAPI.h>
#include "lib/ledController.h"
#include "lib/buttonShieldReceiver.h"


#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedStructInspection"
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#pragma clang diagnostic ignored "-Wmissing-noreturn"


void cycleLights() {
    LedController ledManager;

    unsigned long DELAY = 1000;

    unsigned char ledNumber = 0;

    while (true) {
        ledManager.turnOnLed(ledNumber);
        ledManager.shiftOutLEDs();

        delay(DELAY);

        ledManager.turnOffLed(ledNumber);

        ledNumber++;
        if (ledNumber == NUMBER_OF_LEDS) {
            ledNumber = 0;
        }
    }
}

// To test light marked 3-4 on board use shiftRegister = 3 and value = 4
void singleLightTest(unsigned char shiftRegister, unsigned char value) {
    Serial.begin(9600);
    LedController ledManager;

    ledManager.turnOnLed((unsigned char) (8) * (shiftRegister - (unsigned char) (1)) + (value - (unsigned char) (1)));
    ledManager.shiftOutLEDs();
}

void allLightsTest() {
    LedController ledManager;

    for (unsigned char i = 0; i < NUMBER_OF_LEDS; i++) {
        ledManager.turnOnLed(i);
    }

    ledManager.shiftOutLEDs();
}

class PrintButtonTest : public ButtonPressListener { ;

    void start() {
        Serial.begin(9600);

        ButtonShieldButtonPressReceiver buttonManager = ButtonShieldButtonPressReceiver::create();
        buttonManager.addListener(this);
    }

    void onButtonPressed(unsigned char buttonNumber) override {
        Serial.println("Pressed button: " + String(buttonNumber));
    }
};


class ButtonWithLEDTest : public ButtonPressListener {
    unsigned char buttonNumber = 0;

    LedController ledController;

public:
    void start() {

        ButtonShieldButtonPressReceiver buttonPressReceiver = ButtonShieldButtonPressReceiver::create();
        buttonPressReceiver.addListener(this);

        ledController.turnOnLed(buttonNumber);
        ledController.shiftOutLEDs();
    }

    void onButtonPressed(unsigned char buttonPressed) override {
        if (buttonPressed == buttonNumber) {
            ledController.turnOffLed(buttonPressed);

            buttonNumber++;

            if (buttonNumber == NUMBER_OF_LEDS) {
                buttonNumber = 0;
            }

            ledController.turnOnLed(buttonNumber);
            ledController.shiftOutLEDs();
        }
    }
};


void setup() {
}

void loop() {}


#pragma clang diagnostic pop