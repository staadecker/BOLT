#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>

namespace bluetooth {
  
  void listen();

  bool shouldGoOnline();

  void readReceived();
  void sendPacket(String packetContent);
  void setup();
}

#endif
