//
// Created by machs on 2019-01-19.
//


#include <HardwareSerial.h>
#include <USBAPI.h>
#include "buttonSerialReceiver.h"

void SerialButtonPressReceiver::onRun() {
    if (buttonPressListener != nullptr) {
        while (Serial.available()) {
            char valueRead = static_cast<char>(Serial.read());
            delay(10); // Add delay to allow Serial to work

            if (valueRead == 'P') {
                char buttonNumber[3];

                unsigned char counter = 0;

                while (counter < 2 and Serial.available()) { // Allow up to two characters (if received 2 on Serial)
                    buttonNumber[counter] = static_cast<char>(Serial.read());
                    delay(10);
                    counter++;
                }

                buttonNumber[counter] = '\0';

                Serial.print(F("Serial: Button pressed: "));
                Serial.println(buttonNumber);

                buttonPressListener->onButtonPressed(atoi(buttonNumber));
            }
        }
    }
}
