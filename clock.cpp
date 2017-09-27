#include "clock.h"

#include "screen.h"
#include "controller.h"
#include "const.h"
#include "game.h"
#include "logger.h"
#include <Thread.h>

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
  logger(LOGGER_TYPE_DEBUG, "clock", "clock thread called. Mode : " + String(clock_mode));
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
  clock_thread.ThreadName = "clock";
  clock_thread.setInterval(100);
  clock_thread.enabled = false;
  clock_thread.onRun(clock_callback);
  controller_add(&clock_thread);
}
