#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <Arduino.h>

namespace bluetooth {
  void setup();
  bool isConnected();
  void listen();
  void transmit(String packet);
}

#endif
