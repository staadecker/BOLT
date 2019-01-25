
#include <USBAPI.h>
#include "buttonShieldReceiver.h"
#include "constants.h"

ButtonShieldButtonPressReceiver ButtonShieldButtonPressReceiver::instanceOfButtonShield = ButtonShieldButtonPressReceiver();

ButtonShieldButtonPressReceiver::ButtonShieldButtonPressReceiver() = default;

ButtonShieldButtonPressReceiver &ButtonShieldButtonPressReceiver::create() {
    attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_SHIELD_INTERRUPT), readButtonPress,
                    FALLING); //Attach interrupt for 64 button shield

    return ButtonShieldButtonPressReceiver::instanceOfButtonShield;
}

void ButtonShieldButtonPressReceiver::readButtonPress() {
    unsigned char buttonNumber = 0;

    for (unsigned char i = 0; i < 8; i++) {
        while (digitalRead(PIN_BUTTON_SHIELD_CLOCK) == LOW);  //Wait for clock to go high

        buttonNumber += digitalRead(PIN_BUTTON_SHIELD_DATA);  //If value is high add one

        //If not last time in loop, shift bits
        if (i != 7) {
            buttonNumber = buttonNumber << 1u;
        }

        while (digitalRead(PIN_BUTTON_SHIELD_CLOCK) == HIGH);  //Wait for clock to go low
    }

    if (buttonNumber > 64) {  // Button number starts at 129 when it is a press (rather than a un-press
        if (instanceOfButtonShield.buttonPressListener) {
            instanceOfButtonShield.buttonPressListener->onButtonPressed(buttonNumber - static_cast<unsigned char>(129));
        }
    }
}