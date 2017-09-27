#ifndef FLASHER_H
#define FLASHER_H

#include <Thread.h>

extern Thread flasherThread;

extern bool flashingCurrentState;
extern bool flashed;

void flasher_setup();

const int FLASHER_INTERVAL = 1000;


#endif
