
#include <USBAPI.h>
#include "button-manager.h"
#include "constants.h"

ButtonManager ButtonManager::buttonManager = ButtonManager();

ButtonManager::ButtonManager() = default;

ButtonManager ButtonManager::create() {
    attachInterrupt(digitalPinToInterrupt(P_BUTTON_INTERRUPT), isr,
                    FALLING); //Attach interrupt for 64 button shield
    Serial.println("Created isr");
    return ButtonManager::buttonManager;
}

void ButtonManager::isr() {
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
        if (buttonManager.callback) {
            buttonManager.callback->call(button - uint8_t(129));
        }
    }
}


void ButtonManager::setCallback(ButtonCallbackInterface *callbackArg) {
    callback = callbackArg;
}

void ButtonManager::removeCallback() {
    callback = nullptr;
}