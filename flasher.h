#ifndef FLASHER_H
#define FLASHER_H

#include <Thread.h>

namespace flasher {
  const int FLASHER_INTERVAL = 1000;

  extern Thread flasher_thread;

  extern bool flashingCurrentState;
  extern bool flashed;

  void setupFlasher();
  void callback();
}

#endif
