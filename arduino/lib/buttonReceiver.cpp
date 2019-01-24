//
// Created by machs on 1/21/2019.
//

#include <HardwareSerial.h>
#include "buttonReceiver.h"

void ButtonReceiver::addListener(ButtonPressListener *buttonCallbackInterface) {
    if (listener != nullptr) {
        Serial.println(F("Overwriting listener"));
    }
    listener = buttonCallbackInterface;
}

void ButtonReceiver::removeListener() {
    listener = nullptr;
}
