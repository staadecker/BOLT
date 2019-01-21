//
// Created by machs on 2019-01-19.
//


#include <HardwareSerial.h>
#include <USBAPI.h>
#include "buttonSerialReceiver.h"

ButtonSerialReceiver::ButtonSerialReceiver() {
    threadManager::addThread(this);
}

void ButtonSerialReceiver::runThread() {
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
                listener->buttonPressed(static_cast<uint8_t>(buttonNumber.toInt()));
            }
        }
    }
}

