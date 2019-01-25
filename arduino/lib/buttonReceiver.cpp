//
// Created by machs on 1/21/2019.
//

#include <HardwareSerial.h>
#include "buttonReceiver.h"

void ButtonPressReceiver::addListener(ButtonPressListener *pButtonPressListener) {
    if (buttonPressListener != nullptr) {
        Serial.println(F("Warning: A button press listener already existed. Overwriting it."));
    }
    buttonPressListener = pButtonPressListener;
}

void ButtonPressReceiver::removeListener() {
    buttonPressListener = nullptr;
}
