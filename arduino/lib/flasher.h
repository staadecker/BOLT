#ifndef FLASHER_H
#define FLASHER_H

#include <Arduino.h>

namespace flasher {
    void startFlashing(uint8_t ledNumber);

    void stopFlashing(uint8_t ledNumber);

    void checkFlash();
}

#endif
