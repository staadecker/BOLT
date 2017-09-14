/*
   Thread that displays a timer, stop watch or the time on the led screen

   Use clock_start to start the clock.
   Use clock_stop to stop the clock.
   Use clock_setTimer to set the timer.

   All units in millis
*/

#ifndef CLOCK
#define CLOCK

#include <Thread.h>
#include "screen.h"
#include "controller.h"
#include "const.h"
#include "game.h"

//Forward declarations
uint32_t game_getElapsedTime();
uint32_t game_getRemainingTime();


const uint8_t CLOCK_MODE_CHRONO = 0;
const uint8_t CLOCK_MODE_TIMER = 1;
const uint8_t CLOCK_MODE_TIME = 2;

uint8_t clock_mode;

Thread clock_thread;

String clock_getTime() {
  //TODO
  return String(0);
}

String clock_formatMillis(uint32_t milliSeconds) {
  return String(milliSeconds);
}

void clock_callback() {
  String toDisplay;
  switch (clock_mode) {
    case CLOCK_MODE_CHRONO:
      toDisplay = clock_formatMillis(game_getElapsedTime());
      break;
    case CLOCK_MODE_TIMER:
      toDisplay = clock_formatMillis(game_getRemainingTime());
      break;
    case CLOCK_MODE_TIME:
      toDisplay = clock_getTime();
      break;
  }
  screen_display(toDisplay);
}

//Start the clock in a specific mode
void clock_start(uint8_t mode) {
  clock_mode = mode;
  clock_thread.enabled = true;
}

//Stop the clock
void clock_stop() {
  clock_thread.enabled = false;
}


//Setup the clock thread, enabled = false, add to controller
void clock_setup() {
  clock_thread = Thread();
  clock_thread.setInterval(10);
  clock_thread.enabled = false;
  clock_thread.onRun(clock_callback);
  controller_add(clock_thread);
}

#endif
