#include "timer.h"

#include "screen.h"
#include "controller.h"
#include "const.h"
#include "game.h"
#include "logger.h"
#include <Thread.h>

namespace timer {
  namespace {

    Thread thread = Thread();

    const int INTERVAL = 100;

    String formatMillis(uint32_t milliSeconds) {
      return String(milliSeconds);
    }

    void callback() {
      logger::log(logger::TYPE_DEBUG, "clock", "clock thread called");
      screen::display(formatMillis(game::getRemainingTime()));
    }
  }

  //Start the clock in a specific mode
  void start() {
    thread.enabled = true;
  }

  //Stop the clock
  void stop() {
    thread.enabled = false;
  }


  //Setup the clock thread, enabled = false, add to controller
  void setup() {
    thread.ThreadName = "clock";
    thread.setInterval(INTERVAL);
    thread.enabled = false;
    thread.onRun(callback);
    controller::add(&thread);
  }

}
