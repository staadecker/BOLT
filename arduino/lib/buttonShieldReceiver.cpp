
#include <USBAPI.h>
#include "buttonShieldReceiver.h"
#include "constants.h"

ButtonShieldReceiver ButtonShieldReceiver::buttonShield = ButtonShieldReceiver();

ButtonShieldReceiver::ButtonShieldReceiver() = default;

ButtonShieldReceiver &ButtonShieldReceiver::create() {
    attachInterrupt(digitalPinToInterrupt(P_BUTTON_INTERRUPT), isr,
                    FALLING); //Attach interrupt for 64 button shield
    Serial.println(F("Created isr"));
    return ButtonShieldReceiver::buttonShield;
}

void ButtonShieldReceiver::isr() {
    uint8_t button = 0;

    for (int i = 0; i < 8; i++) {

        while (digitalRead(P_BUTTON_CLOCK) == LOW);  //Wait for clock to go high

        button += digitalRead(P_BUTTON_DATA);  //If value is high add one

        //If not last time in loop, shift bits
        if (i != 7) {
            button = button << 1u; //u for unsigned
        }

        while (digitalRead(P_BUTTON_CLOCK) == HIGH);  //Wait for clock to go low
    }

    if (button > 64) {  // If button pressed
        if (buttonShield.listener) {
            buttonShield.listener->buttonPressed(button - uint8_t(129));
        }
    }
}

void ButtonShieldReceiver::checkForButtonPress() {}