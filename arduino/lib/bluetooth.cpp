#include <USBAPI.h>
#include "bluetooth.h"

constexpr char Bluetooth::BEGIN_PACKET[3]; //Do not remove or else doesn't compile.

Bluetooth::Bluetooth(LedController &ledArg, ButtonReceiver *buttonReceiver, DoneGameCallback *doneGameCallback)
        : ledManager(ledArg), doneGameCallback(doneGameCallback), buttonReceiver(buttonReceiver) {
    BT.begin(9600);
}

bool Bluetooth::shouldGoOnline() {
    return containsBeginPacket(readReceived());
}

void Bluetooth::goOnline() {
    buttonReceiver->addListener(this);
    threadManager::addThread(this);
}

void Bluetooth::runThread() {
    analyzeContent(readReceived());
}

bool Bluetooth::containsBeginPacket(const char *content) {
    return strstr(content, BEGIN_PACKET);
}

char *Bluetooth::readReceived() {
    static char content[256];

    unsigned char index = 0;
    while (BT.available() and index < 255) {
        content[index] = char(BT.read());
        delay(10);
        index++;
    }

    content[index + 1] = '\0';

    return content;
}


void Bluetooth::sendPacket(const char *packetContent) {
    Serial.print("Sent packet: ");
    Serial.println(packetContent);

    char packet[sizeof(packetContent) + 2];
    packet[0] = START_OF_PACKET;
    strcat(packet, packetContent);
    packet[strlen(packet)] = END_OF_PACKET;
    packet[strlen(packet) + 1] = '\0';

    BT.write(packet);

    if (acknowledgeTimeout == -1u) {
        acknowledgeTimeout = millis() + ACKNOWLEDGE_TIMEOUT;
    }
}


void Bluetooth::analyzeContent(const char *content) {
    uint8_t index = 0;

    while (index < strlen(content)) {
        if (content[index] == START_OF_PACKET) {
            bool packetDone = false;

            while (!packetDone) {
                if (index >= strlen(content)) {
                    Serial.println("Did not receive end of packet byte");
                    break;
                }

                switch (content[index]) {
                    case C_BEGIN: {
                        index++;
                        break;
                    }
                    case C_END: {
                        index++;
                        exitBluetoothMode();
                        break;
                    }
                    case C_TURN_ON_LED: {
                        char ledNumber[3];
                        ledNumber[0] = content[index + 1];
                        ledNumber[1] = content[index + 2];
                        ledNumber[2] = '\0';

                        ledManager.turnOn(atoi(ledNumber));
                        index += 3;
                        break;
                    }

                    case C_TURN_OFF_LED: {
                        char ledNumber[3];
                        ledNumber[0] = content[index + 1];
                        ledNumber[1] = content[index + 2];
                        ledNumber[2] = '\0';


                        ledManager.turnOff(static_cast<const unsigned char &>(atoi(ledNumber)));
                        index += 3;
                        break;
                    }
                    case C_SHIFT_OUT: {
                        ledManager.shiftOut();
                        index++;
                        break;
                    }
                    case END_OF_PACKET: {
                        index++;
                        packetDone = true;
                        BT.write(ACKNOWLEDGE);
                        break;
                    }
                    default: {
                        packetDone = true; //This will exit the inner loop and result in the error being printed in the loop bellow.
                    }
                }
            }
        } else if (content[index] == ACKNOWLEDGE) {
            acknowledgeTimeout = -1uL;
            index++;
        } else {
            Serial.print("Could not parse this content: ");
            Serial.println(content);
            break;
        }
    }
}

void Bluetooth::buttonPressed(const unsigned char &buttonPressed) {
    char packetContent[4];
    packetContent[0] = Bluetooth::C_BUTTON_PRESS;

    char buttonNumber[3];
    sprintf(buttonNumber, "%05d", buttonPressed);
    strcat(packetContent, buttonNumber);

    sendPacket(packetContent);
}

void Bluetooth::exitBluetoothMode() {
    threadManager::removeThread(this);
    buttonReceiver->removeListener();
    doneGameCallback->done();
}