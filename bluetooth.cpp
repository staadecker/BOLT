#include "bluetooth.h"

#include "constants.h"
#include "led.h"
#include "logger.h"
#include "button.h"

#include <SoftwareSerial.h>

namespace bluetooth {
  namespace {
    SoftwareSerial BT(constants::P_SOFTWARE_SERIAL_RX, constants::P_SOFTWARE_SERIAL_TX);

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

    const String BLUETOOTH_PIN = "1234";

    bool connection = true; //Assume already connected. On setup will change to false if no response to the AT command is received.

    volatile long acknowledgeTimeout = NO_TIMEOUT;

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

      unsigned long timeOutTime = millis() + PACKET_TIMEOUT;

      while (millis() < timeOutTime) {
        if (BT.available()) {
          char newByte = BT.read();
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

      logger::log(logger::TYPE_WARNING, "bluetooth", "Timout while reading packet content: " + packet);
      return ""; // If while loop ended because off timeout, end game
    }
  }

  void checkSerial() {
    while (Serial.available()) {
      int value = Serial.read();
      delay(10);
      if (value == 80) {
        String buttonNumber = "";
        while (Serial.available()) {
          buttonNumber += (char) Serial.read();
          delay(10);
        }
        Serial.println("Debug button pressed : " + buttonNumber);
        button::buttonPressedCallback(buttonNumber.toInt());
      }
    }
  }

  void readReceived() {
    if (BT.available()) {
      String unknown = "";

      while (BT.available()) {
        char newByte = char(BT.read());
        delay(10);

        switch (newByte) {
          case ACKNOWLEDGE:
            logger::log(logger::TYPE_INFO, "bluetooth", "Received acknowledge");
            acknowledgeTimeout = NO_TIMEOUT;
            break;
          case START_OF_PACKET:
            processPacketContent(getPacketContent());
            break;
          default:
            unknown += newByte;

        }
      }

      if (unknown.equals("OK+LOST")) {
        connection = false;
      } else if (unknown.equals("OK+CONN")) {
        connection = true;
      } else if (unknown.equals("OK+Set:1")) {
        connection = false;
      } else if (not unknown.equals("")) {
        logger::log(logger::TYPE_WARNING, "bluetooth", "Received unknown bytes: " + unknown);
      }
    }

    if (constants::IS_DEBUGGING) {
      checkSerial();
    }
  }

  void listen() {
    while (connection) {
      readReceived();
      if (acknowledgeTimeout != NO_TIMEOUT and millis() > acknowledgeTimeout) {
        connection = false;
        acknowledgeTimeout = NO_TIMEOUT;
        logger::log(logger::TYPE_WARNING, "bluetooth", "No acknowledge received. Disconnecting");
      }
    }
  }

  void sendPacket(String packetContent) {
    logger::log(logger::TYPE_INFO, "bluetooth", "Sent packet : " + packetContent);
    BT.write(START_OF_PACKET);
    for (int i = 0 ; i < packetContent.length(); i++) {
      BT.write(packetContent.charAt(i));
    }
    BT.write(END_OF_PACKET);

    if (acknowledgeTimeout == NO_TIMEOUT) {
      acknowledgeTimeout = millis() + ACKNOWLEDGE_TIMEOUT;
    }
  }

  bool shouldGoOnline() {
    readReceived();
    return connection;
  }

  void setup() {
    BT.begin(9600);
    BT.write("AT+NOTI1");
    delay(1000); //Delay to allow chip to send response to AT command to see if device is connected
  }
}
