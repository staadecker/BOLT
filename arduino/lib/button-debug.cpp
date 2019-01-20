//
// Created by machs on 2019-01-19.
//

#include <HardwareSerial.h>
#include <USBAPI.h>
#include "button-debug.h"

void ButtonDebug::setCallback(ButtonCallbackInterface *buttonCallbackInterface) {
    listener = buttonCallbackInterface;
    threaderId = threader::addCallback(this);
}

void ButtonDebug::removeCallback() {
    threader::removeCallback(threaderId);
    listener = nullptr;
}

void ButtonDebug::call() {
    if (listener and Serial.available()) {
        while (Serial.available()) {
            int value = Serial.read();
            delay(10);
            if (value == 80) {
                String buttonNumber = "";
                while (Serial.available()) {
                    buttonNumber += (char) Serial.read();
                    delay(10);
                }
                //Serial.println("Debug button pressed : " + buttonNumber);
                listener->call(static_cast<uint8_t>(buttonNumber.toInt()));
            }
        }
    }
}

