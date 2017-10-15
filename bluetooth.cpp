#include "bluetooth.h"

#include "controller.h"
#include "const.h"
#include "led.h"
#include "logger.h"

#include <SoftwareSerial.h>

namespace bluetooth {
  namespace {
    SoftwareSerial mySerial(constants::P_SOFTWARE_SERIAL_RX, constants::P_SOFTWARE_SERIAL_TX);

    const char START_OF_PACKET = 0x02;  //Start of text
    const char END_OF_PACKET = 0x03;  //End of text
    const char ACKNOWLEDGE = 0x06; //Acknowledge

    const String REQUEST_PACKET_CONTENT = "R";

    const char C_DISCONNECT = 0x44; //"D"
    const char C_TURN_ON_LED = 0x4C; //"L"
    const char C_TURN_OFF_LED = 0x4F; //"O"

    const String BLUETOOTH_PIN = "756945";

    const int CONNECTION_INTERVAL = 1000;

    bool connection = false;

    void acknowledgePacket() {
      mySerial.print(ACKNOWLEDGE);
    }

    void processPacketContent(String packetContent) {
      char command = packetContent[0];
      String argument = packetContent.substring(1);

      switch (command) {
        case C_DISCONNECT:
          connection = false;
          break;
        case C_TURN_ON_LED:
          led::turnOn(argument.toInt());
          break;
        case C_TURN_OFF_LED:
          led::turnOff(argument.toInt());
          break;
        default:
          logger::log(logger::TYPE_ERROR, "bluetooth", "can't parse packet : " + packetContent);
      }
    }

    String getPacketContent() {
      String packet = "";

      while (true) {
        if (Serial.available()) {

          char newByte = Serial.read();

          if (newByte = END_OF_PACKET) {
            break;
          } else {
            packet += newByte;
          }
        }
      }

      return packet;
    }

    void readReceived() {
      String unknown = "";

      while (Serial.available()) {
        char newByte = Serial.read();

        switch (newByte) {
          case ACKNOWLEDGE:
            connection = true;
            break;
          case START_OF_PACKET:
            processPacketContent(getPacketContent());
            acknowledgePacket();
            break;
          default:
            unknown += newByte;
        }
      }

      if (not unknown.equals("")) {
        logger::log(logger::TYPE_WARNING, "bluetooth", "Unknown RX bytes : " + unknown);
      }
    }
  }

  void sendPacket(String packetContent) {
    mySerial.print(START_OF_PACKET + packetContent + END_OF_PACKET);
  }

  void listen() {
    while (connection) {
      readReceived();
    }
  }

  bool isConnected() {
    return connection;
  }

  void testConnection() {
    sendPacket(REQUEST_PACKET_CONTENT);
    readReceived();
  }

  void setup() {
    mySerial.begin(9600);
    mySerial.print("AT+PIN:" + BLUETOOTH_PIN); //Set PIN to be same as phone
  }
}



