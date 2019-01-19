#include "button-manager.h"
#include "constants.h"
#include "bluetooth.h"

ButtonManager ButtonManager::buttonManager = ButtonManager();

ButtonManager::ButtonManager() = default;

void ButtonManager::setup() {
    attachInterrupt(digitalPinToInterrupt(P_BUTTON_INTERRUPT), staticIsr,
                    FALLING); //Attach interrupt for 64 button shield
}

ButtonManager ButtonManager::get() {
    return ButtonManager::buttonManager;
}

void ButtonManager::staticIsr() {
    buttonManager.isr();
}

//Method to call when the the interrupt pin is falling
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
        buttonPressed = button - uint8_t(129);
        buttonWasPressed = true;
        callback->call(buttonPressed);
    }
}

//Checks whether the button got pressed
bool ButtonManager::isPressed(int buttonToCheck) {
    //If button pressed is the button to check return true
    return buttonWasPressed and buttonPressed == buttonToCheck;
}

void ButtonManager::clearLast() {
    buttonWasPressed = false;
}

int8_t ButtonManager::getButtonLastPressed() {
    if (!buttonWasPressed) {
        return -1;
    }

    return buttonPressed;
}

void ButtonManager::setCallback(ButtonCallbackInterface *callbackArg) {
    callback = callbackArg;
}