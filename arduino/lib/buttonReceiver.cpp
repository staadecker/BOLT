//
// Created by machs on 1/21/2019.
//

#include "buttonReceiver.h"

void ButtonReceiver::addListener(ButtonPressListener *buttonCallbackInterface) {
    listener = buttonCallbackInterface;
}

void ButtonReceiver::removeListener() {
    listener = nullptr;
}
