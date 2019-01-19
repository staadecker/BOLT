#include "bluetooth.h"

#include "logger.h"


Bluetooth::Bluetooth(LedManager ledArg, ButtonInterface *buttonInterface) : ledManager(ledArg) {
    BT.begin(9600);
    BT.write("AT+NOTI1");
    delay(1000); //Delay to allow BT chip to send response to AT command to see if device is connected
    buttonInterface->setCallback(this);
}

void Bluetooth::acknowledgePacket() {
    BT.write(ACKNOWLEDGE);
    log(TYPE_INFO, "bluetooth", "Acknowledged packet");
}

void Bluetooth::processPacketContent(String packetContent) {
    if (packetContent.equals("")) {
        return;
    }

    char command = packetContent[0];
    String argument = packetContent.substring(1);

    if (command == C_BEGIN) {
        isOnline = true;
        acknowledgePacket();
    }

        //Only analyse packet if a connection is already made
    else if (isOnline) {
        switch (command) {
            case C_END:
                isOnline = false;
                break;
            case C_TURN_ON_LED:
                ledManager.turnOn(static_cast<uint8_t>(argument.toInt()));
                break;
            case C_TURN_OFF_LED:
                ledManager.turnOff(static_cast<uint8_t>(argument.toInt()));
                break;
            case C_SHIFT_OUT:
                ledManager.shiftOut();
                break;
            default:
                log(TYPE_ERROR, "bluetooth", "can't parse packet : " + packetContent);
        }

        acknowledgePacket();
    }
}

String Bluetooth::getPacketContent() {
    String packet = "";

    unsigned long timeOutTime = millis() + PACKET_TIMEOUT;

    while (millis() < timeOutTime) {
        if (BT.available()) {
            auto newByte = static_cast<char>(BT.read());
            delay(10);

            timeOutTime = millis() + PACKET_TIMEOUT;

            if (newByte == END_OF_PACKET) {
                log(TYPE_INFO, "bluetooth", "Received packet : " + packet);
                return packet;
            } else {
                packet += newByte;
            }
        }
    }

    log(TYPE_WARNING, "bluetooth", "Timeout while reading packet content: " + packet);
    return ""; // If while loop ended because off timeout, end game
}


void Bluetooth::checkSerial() {
    // TODO Fix check serial
//  while (Serial.available()) {
//    int value = Serial.read();
//    delay(10);
//    if (value == 80) {
//      String buttonNumber = "";
//      while (Serial.available()) {
//        buttonNumber += (char) Serial.read();
//        delay(10);
//      }
//      Serial.println("Debug button pressed : " + buttonNumber);
//      button::buttonPressedCallback(buttonNumber.toInt());
//    }
//  }
}

void Bluetooth::readReceived() {
    if (BT.available()) {
        String unknown = "";

        while (BT.available()) {
            auto newByte = char(BT.read());
            delay(10);

            switch (newByte) {
                case ACKNOWLEDGE:
                    log(TYPE_INFO, "bluetooth", "Received acknowledge");
                    activeTimeOut = false;
                    break;
                case START_OF_PACKET:
                    processPacketContent(getPacketContent());
                    break;
                default:
                    unknown += newByte;

            }
        }

        if (unknown.equals("OK+LOST")) {
            isOnline = false;
        } else if (unknown.equals("OK+CONN")) {
            log(TYPE_INFO, "bluetooth", "A device connected to the board");
        } else if (unknown.equals("OK+Set:1")) {
            //Response from AT command. To ignore.
        } else if (not unknown.equals("")) {
            log(TYPE_WARNING, "bluetooth", "Received unknown bytes: " + unknown);
        }
    }

    //if (IS_DEBUGGING) {
    //   checkSerial();
    //}
}

void Bluetooth::listen() {
    while (isOnline) {
        readReceived();
        if (activeTimeOut and millis() > acknowledgeTimeout) {
            isOnline = false;
            activeTimeOut = false;
            log(TYPE_WARNING, "bluetooth", "No acknowledge received. Disconnecting");
        }
    }
}

void Bluetooth::sendPacket(String packetContent) {
    if (isOnline) {
        log(TYPE_INFO, "bluetooth", "Sent packet : " + packetContent);
        BT.write(START_OF_PACKET);
        for (unsigned int i = 0; i < packetContent.length(); i++) {
            BT.write(static_cast<uint8_t>(packetContent.charAt(i)));
        }
        BT.write(END_OF_PACKET);

        if (!activeTimeOut) {
            acknowledgeTimeout = millis() + ACKNOWLEDGE_TIMEOUT;
        }
    }
}

bool Bluetooth::shouldGoOnline() {
    readReceived();
    return isOnline;
}

void Bluetooth::call(uint8_t buttonPressed) {
    sendPacket(Bluetooth::C_BUTTON_PRESS + String(buttonPressed));
}