#ifndef FLASHER_H
#define FLASHER_H

#include <Thread.h>

namespace flasher {
  void flash(uint8_t ledNumber);
  void stopFlashing(uint8_t ledNumber);
  void run();
  void setup();
}

#endif
