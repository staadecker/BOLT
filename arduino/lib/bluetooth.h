#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>

namespace bluetooth {

    const char C_BUTTON_PRESS = 0x50; // "P"

    void listen();

    bool shouldGoOnline();

    void readReceived();

    void sendPacket(String packetContent);

    void setup();
}

#endif
