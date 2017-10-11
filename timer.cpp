#include "timer.h"

#include "screen.h"
#include "controller.h"
#include "const.h"
#include "game.h"
#include "logger.h"
#include <Thread.h>

namespace timer {
  namespace {
    uint8_t mode;

    Thread thread;

    const int INTERVAL = 100;
    
    String getTime() {
      //TODO
      return String(0);
    }

    String formatMillis(uint32_t milliSeconds) {
      return String(milliSeconds);
    }

    void callback() {
      logger::log(logger::TYPE_DEBUG, "clock", "clock thread called. Mode : " + String(mode));
      String toDisplay;
      switch (mode) {
        case MODE_CHRONO:
          toDisplay = formatMillis(game::getElapsedTime());
          break;
        case MODE_TIMER:
          toDisplay = formatMillis(game::getRemainingTime());
          break;
        case MODE_TIME:
          toDisplay = getTime();
          break;
      }
      screen::display(toDisplay);
    }
  }

  //Start the clock in a specific mode
  void start(uint8_t modeToSet) {
    mode = modeToSet;
    thread.enabled = true;
  }

  //Stop the clock
  void stop() {
    thread.enabled = false;
  }


  //Setup the clock thread, enabled = false, add to controller
  void setup() {
    thread = Thread();
    thread.ThreadName = "clock";
    thread.setInterval(INTERVAL);
    thread.enabled = false;
    thread.onRun(callback);
    controller::add(&thread);
  }

}
