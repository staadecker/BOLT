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

    bool connection = false;

    void parsePacket(String packet) {

      switch (packet[0]) {
        case C_DISCONNECT:
          connection = false;
          break;
        case C_TURN_ON_LED:
          led::turnOn(packet.substring(1).toInt());
          break;
        case C_TURN_OFF_LED:
          led::turnOff(packet.substring(1).toInt());
          break;
        case C_START:
          connection = true;
          break;
        default:
          logger::log(logger::TYPE_ERROR, "bluetooth", "unkown command : " + packet);
      }

      transmit(ACKNOWLEDGE_COMMAND);
    }
  }

  void setup() {
    mySerial.begin(9600);
    transmit("AT+PIN:" + BLUETOOTH_PIN);
    
    listeningThread.setInterval(LISTENING_INTERVAL);
    listeningThread.onRun(listen);
    listeningThread.ThreadName = "bluetooth";
    controller::add(&listeningThread);
  }

  void listen() {
    char receivedByte;
    String packet = "";

    while (mySerial.available()) {

      receivedByte = mySerial.read();

      Serial.print(receivedByte);

      if (receivedByte == END_OF_PACKET) {
        parsePacket(packet);
        packet = "";
      }
      else {
        packet += receivedByte;
      }
    }
  }

  void transmit(String packet) {
    mySerial.print(packet);
  }

  bool isConnected() {
    return connection;
  }
}



