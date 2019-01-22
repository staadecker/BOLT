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
                char buttonNumber[3];
                unsigned char index = 0;
                while (index < 2 and Serial.available()) {
                    buttonNumber[index] = (char) Serial.read();
                    delay(10);
                    index++;
                }
                listener->buttonPressed(static_cast<uint8_t>(atoi(buttonNumber)));
            }
        }
    }
}

