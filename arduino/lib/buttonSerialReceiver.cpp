//
// Created by machs on 2019-01-19.
//


#include <HardwareSerial.h>
#include <USBAPI.h>
#include "buttonSerialReceiver.h"


void ButtonSerialReceiver::checkForButtonPress() {
    if (listener != nullptr and Serial.available()) {
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
                buttonNumber[index] = '\0';
                Serial.print(F("Received button press: "));
                Serial.println(buttonNumber);
                listener->buttonPressed(atoi(buttonNumber));
            }
        }
    }
}
