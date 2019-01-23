#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <SoftwareSerial.h>
#include "buttonReceiver.h"
#include "ledController.h"

class Bluetooth : public ButtonPressListener {
    static const char START_OF_PACKET = 0x02;  //Start of packet
    static const char END_OF_PACKET = 0x03;  //End of packet
    static const char ACKNOWLEDGE = 0x06; //Acknowledge

    static const char C_BEGIN = 0x42; //"B"
    static const char C_END = 0x45; //"E"
    static const char C_TURN_OFF_LED = 0x49; //"I"
    static const char C_TURN_ON_LED = 0x4F; //"O"
    static const char C_SHIFT_OUT = 0x53; //"S"
    static const char C_BUTTON_PRESS = 0x50; // "P"

    static constexpr char BEGIN_PACKET[] = {START_OF_PACKET, C_BEGIN, END_OF_PACKET};

    static const unsigned int ACKNOWLEDGE_TIMEOUT = 2000;

    SoftwareSerial BT = SoftwareSerial(P_SOFTWARE_SERIAL_RX, P_SOFTWARE_SERIAL_TX);

    volatile unsigned long acknowledgeTimeout = 0;

    LedController &ledManager;
    ButtonReceiver *buttonReceiver;

    void buttonPressed(const uint8_t &buttonPressed) override;

    char *readReceived();

    bool containsBeginPacket(const char *content);

    void sendPacket(const char *packetContent);

    bool analyzeContent(const char *content);
public:
    Bluetooth(LedController &ledArg, ButtonReceiver *buttonReceiver);

    void goOnline();

    bool shouldGoOnline();
};

#endif
