#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <SoftwareSerial.h>
#include "buttonReceiver.h"
#include "ledController.h"
#include "runnablesManager.h"
#include "returnToStartingStateCallback.h"

//This class is responsible for managing the bluetooth interactions with a phone
//When created the class opens a Software serial connection with the Bluetooth chip
//The class has two modes. The first is when it's just listening to see if a phone has connected
//The second is once the phone has connected and the game is running
//The start and end packet indicate when the modes should change
//The bluetooth packet format can be found in bluetooth-protocol.md in the main folder of the repo
class BluetoothManager : public ButtonPressListener, public Runnable {
    //Constants used to create and parse packets
    static const char START_OF_PACKET = 0x01;  //Start of packet
    static const char END_OF_PACKET = 0x04;  //End of packet
    static const char ACKNOWLEDGE_BYTE = 0x06; //Acknowledge
    static const char BEGIN_CONNECTION = 'B';
    static const char END_CONNECTION = 'E';
    static const char TURN_OFF_LED = 'I';
    static const char TURN_ON_LED = 'O';
    static const char SHIFT_OUT = 'S';
    static const char BUTTON_PRESSED = 'P';
    static constexpr char BEGIN_CONNECTION_PACKET[] = {START_OF_PACKET, BEGIN_CONNECTION, END_OF_PACKET};

    SoftwareSerial BtSerial = SoftwareSerial(PIN_BLUETOOTH_SERIAL_RX, PIN_BLUETOOTH_SERIAL_TX);

    LedController &ledManager;
    ReturnToStartStateCallback *returnToStartingStateCallback;
    ButtonPressReceiver *buttonPressReceiver;

    void onButtonPressed(unsigned char buttonPressed) override;

    char *readBluetoothSerial();

    bool doesContainBeginConnectionPacket(const char *receivedData);

    void sendPacket(const char *packetData);

    void onRun() override;

    void returnToStartingState();

    //Most complicated piece of code in class
    //Iterates over the bytes in the receivedData one-by-one and run the required methods
    //Prints to serial error message if received data does not match format
    void parseReceivedData(const char *receivedData);

public:
    BluetoothManager(LedController &ledArg, ButtonPressReceiver *buttonPressReceiver,
                     ReturnToStartStateCallback *returnToStartingStateCallback);

    void goInBluetoothState();

    bool shouldGoInBluetoothState();
};

#endif
