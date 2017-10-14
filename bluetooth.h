#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>

namespace bluetooth {

  void sendPacket(String packet);
  void listen();

  bool isConnected();
  void testConnection();
  void setup();
}

#endif
