#include "bluetooth.h"

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

    const char C_BEGIN = 0x42; //"B"
    const char C_END = 0x45; //"E"
    const char C_TURN_OFF_LED = 0x49; //"I"
    const char C_TURN_ON_LED = 0x4F; //"O"
    const char C_SHIFT_OUT = 0x53; //"S"

    const unsigned long PACKET_TIMEOUT = 1000;
    const unsigned int ACKNOWLEDGE_TIMEOUT = 2000;
    const long NO_TIMEOUT = -1;

    const String BLUETOOTH_PIN = "756945";

    bool connection = false;

    volatile long acknowledgeTimeout = NO_TIMEOUT;

    void acknowledgePacket() {
      mySerial.print(ACKNOWLEDGE);
    }

    void processPacketContent(String packetContent) {
      char command = packetContent[0];
      String argument = packetContent.substring(1);

      if (command = C_BEGIN) {
        connection = true;
        acknowledgePacket();
      }

      else if (connection) {
        switch (command) {
          case C_END:
            connection = false;
            break;
          case C_TURN_ON_LED:
            led::turnOn(argument.toInt());
            break;
          case C_TURN_OFF_LED:
            led::turnOff(argument.toInt());
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

      unsigned long timeAtLastByte = millis();

      while (millis() < timeAtLastByte + PACKET_TIMEOUT) {
        if (Serial.available()) {

          char newByte = Serial.read();
          timeAtLastByte = millis();

          if (newByte == END_OF_PACKET) {
            return packet;
          } else {
            packet += newByte;
          }
        }
      }
      logger::log(logger::TYPE_WARNING, "bluetooth", "Timout while reading packet content");
      return String(C_END); // If while loop ended because off timeout, end game
    }
  }

  void readReceived() {
    String unknown = "";

    while (Serial.available()) {
      char newByte = Serial.read();

      switch (newByte) {
        case ACKNOWLEDGE:
          acknowledgeTimeout = NO_TIMEOUT;
          break;
        case START_OF_PACKET:
          processPacketContent(getPacketContent());
          break;
        default:
          unknown += newByte;
      }
    }

    if (not unknown.equals("")) {
      logger::log(logger::TYPE_WARNING, "bluetooth", "Unknown RX bytes : " + unknown);
    }
  }

  void listen() {
    while (connection) {
      readReceived();
      if (acknowledgeTimeout != NO_TIMEOUT and millis() > acknowledgeTimeout) {
        connection = false;
        acknowledgeTimeout = NO_TIMEOUT;
      }
    }
  }

  void sendPacket(String packetContent) {
    mySerial.print(START_OF_PACKET + packetContent + END_OF_PACKET);
    if (acknowledgeTimeout == NO_TIMEOUT) {
      acknowledgeTimeout = millis() + ACKNOWLEDGE_TIMEOUT;
    }
  }

  bool shouldGoOnline() {
    return connection;
  }

  void setup() {
    mySerial.begin(9600);
    mySerial.print("AT+PIN:" + BLUETOOTH_PIN); //Set PIN to be same as phone
  }
}
