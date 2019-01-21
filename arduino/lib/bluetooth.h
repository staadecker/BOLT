#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <SoftwareSerial.h>
#include "buttonReceiver.h"
#include "ledController.h"

class Bluetooth : public ButtonPressListener {
    SoftwareSerial BT = SoftwareSerial(P_SOFTWARE_SERIAL_RX, P_SOFTWARE_SERIAL_TX);

    LedController ledManager;

    static const char START_OF_PACKET = 0x02;  //Start of text
    static const char END_OF_PACKET = 0x03;  //End of text
    static const char ACKNOWLEDGE = 0x06; //Acknowledge

    static const char C_BEGIN = 0x42; //"B"
    static const char C_END = 0x45; //"E"
    static const char C_TURN_OFF_LED = 0x49; //"I"
    static const char C_TURN_ON_LED = 0x4F; //"O"
    static const char C_SHIFT_OUT = 0x53; //"S"

    static const unsigned long PACKET_TIMEOUT = 1000;
    static const unsigned int ACKNOWLEDGE_TIMEOUT = 2000;

    bool isOnline = false;

    bool activeTimeOut = false;
    volatile unsigned long acknowledgeTimeout = 0;

    void acknowledgePacket();

    void processPacketContent(const String &packetContent);

    String getPacketContent();

    void buttonPressed(const uint8_t &buttonPressed) override;

public:
    Bluetooth(const LedController &ledArg, ButtonReceiver *buttonInterface);

    static const char C_BUTTON_PRESS = 0x50; // "P"

    void listen();

    bool shouldGoOnline();

    void readReceived();

    void sendPacket(const String &packetContent);


};

#endif
