#ifndef FLASHER_H
#define FLASHER_H

#include <Thread.h>

const int FLASHER_INTERVAL = 1000;

extern Thread flasher_thread;

extern bool flashingCurrentState;
extern bool flashed;

void flasher_setup();
void flasher_callback();

#endif
