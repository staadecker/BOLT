#include "bluetooth.h"

#include "controller.h"
#include "const.h"
#include "led.h"
#include "logger.h"

#include <Thread.h>
#include <SoftwareSerial.h>

namespace bluetooth {
  namespace {
    SoftwareSerial mySerial(constants::P_SOFTWARE_SERIAL_RX, constants::P_SOFTWARE_SERIAL_TX);

    Thread listeningThread = Thread();

    const int LISTENING_INTERVAL = 2000;

    const char END_OF_PACKET = 0x0D;  // Carriage return

    const char C_DISCONNECT = 0x44; //"D"
    const char C_TURN_ON_LED = 0x4C; //"L"
    const char C_TURN_OFF_LED = 0x4F; //"O"
    const char C_START = 0x53; //"S"

    const String ACKNOWLEDGE_COMMAND = "A";

    const String BLUETOOTH_PIN = "756945";

    bool shouldListen = false;

    String currentPacket = "";

    void parsePacket(String packet) {

      char command = packet[0];
      String argument = packet.substring(1);

      switch (command) {
        case C_DISCONNECT:
          shouldListen = false;
          break;
        case C_TURN_ON_LED:
          led::turnOn(argument.toInt());
          break;
        case C_TURN_OFF_LED:
          led::turnOff(argument.toInt());
          break;
        case C_START:
          shouldListen = true;
          break;
        default:
          logger::log(logger::TYPE_ERROR, "bluetooth", "unkown command : " + packet);
      }

      transmit(ACKNOWLEDGE_COMMAND);
    }

    void readReceivedBytes() {
      char receivedByte;

      while (mySerial.available() > 0) {

        receivedByte = mySerial.read();

        if (receivedByte == END_OF_PACKET) {
          parsePacket(currentPacket);
          currentPacket = "";
        }
        else {
          currentPacket += receivedByte;
        }
      }
    }
  }

  void setup() {
    mySerial.begin(9600);
    transmit("AT+PIN:" + BLUETOOTH_PIN);

    listeningThread.setInterval(LISTENING_INTERVAL);
    listeningThread.onRun(readReceivedBytes);
    listeningThread.ThreadName = "bluetooth";
    controller::add(&listeningThread);
  }

  void listen() {
    while (shouldListen) {
      readReceivedBytes();
    }
  }

  void transmit(String packet) {
    mySerial.print(packet);
  }

  bool isConnected() {
    return shouldListen;
  }
}



