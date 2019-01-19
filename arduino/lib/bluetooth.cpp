#include "bluetooth.h"

#include "constants.h"
#include "led.h"
#include "logger.h"

#include <SoftwareSerial.h>

namespace bluetooth {
    namespace {
        SoftwareSerial BT(P_SOFTWARE_SERIAL_RX, P_SOFTWARE_SERIAL_TX);

        const char START_OF_PACKET = 0x02;  //Start of text
        const char END_OF_PACKET = 0x03;  //End of text
        const char ACKNOWLEDGE = 0x06; //Acknowledge

        const char C_BEGIN = 0x42; //"B"
        const char C_END = 0x45; //"E"
        const char C_TURN_OFF_LED = 0x49; //"I"
        const char C_TURN_ON_LED = 0x4F; //"O"
        const char C_SHIFT_OUT = 0x53; //"S"

        const unsigned long PACKET_TIMEOUT = 1000;
        const unsigned int ACKNOWLEDGE_TIMEOUT = 2000;

        bool isOnline = false;

        bool activeTimeOut = false;
        volatile unsigned long acknowledgeTimeout = 0;

        void acknowledgePacket() {
            BT.write(ACKNOWLEDGE);
            logger::log(logger::TYPE_INFO, "bluetooth", "Acknowledged packet");
        }

        void processPacketContent(String packetContent) {
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
                        led::turnOn(static_cast<uint8_t>(argument.toInt()));
                        break;
                    case C_TURN_OFF_LED:
                        led::turnOff(static_cast<uint8_t>(argument.toInt()));
                        break;
                    case C_SHIFT_OUT:
                        led::shiftOut();
                        break;
                    default:
                        logger::log(logger::TYPE_ERROR, "bluetooth", "can't parse packet : " + packetContent);
                }

                acknowledgePacket();
            }
        }

        String getPacketContent() {
            String packet = "";

            unsigned long timeOutTime = millis() + PACKET_TIMEOUT;

            while (millis() < timeOutTime) {
                if (BT.available()) {
                    auto newByte = static_cast<char>(BT.read());
                    delay(10);

                    timeOutTime = millis() + PACKET_TIMEOUT;

                    if (newByte == END_OF_PACKET) {
                        logger::log(logger::TYPE_INFO, "bluetooth", "Received packet : " + packet);
                        return packet;
                    } else {
                        packet += newByte;
                    }
                }
            }

            logger::log(logger::TYPE_WARNING, "bluetooth", "Timeout while reading packet content: " + packet);
            return ""; // If while loop ended because off timeout, end game
        }
    }

    void checkSerial() {
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

    void readReceived() {
        if (BT.available()) {
            String unknown = "";

            while (BT.available()) {
                auto newByte = char(BT.read());
                delay(10);

                switch (newByte) {
                    case ACKNOWLEDGE:
                        logger::log(logger::TYPE_INFO, "bluetooth", "Received acknowledge");
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
                logger::log(logger::TYPE_INFO, "bluetooth", "A device connected to the board");
            } else if (unknown.equals("OK+Set:1")) {
                //Response from AT command. To ignore.
            } else if (not unknown.equals("")) {
                logger::log(logger::TYPE_WARNING, "bluetooth", "Received unknown bytes: " + unknown);
            }
        }

        if (IS_DEBUGGING) {
            checkSerial();
        }
    }

    void listen() {
        while (isOnline) {
            readReceived();
            if (activeTimeOut and millis() > acknowledgeTimeout) {
                isOnline = false;
                activeTimeOut = false;
                logger::log(logger::TYPE_WARNING, "bluetooth", "No acknowledge received. Disconnecting");
            }
        }
    }

    void sendPacket(String packetContent) {
        if (isOnline) {
            logger::log(logger::TYPE_INFO, "bluetooth", "Sent packet : " + packetContent);
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

    bool shouldGoOnline() {
        readReceived();
        return isOnline;
    }

    void setup() {
        if (IS_BLUETOOTH_CHIP_CONNECTED) {
            BT.begin(9600);
            BT.write("AT+NOTI1");
            delay(1000); //Delay to allow BT chip to send response to AT command to see if device is connected
        }
    }
}
