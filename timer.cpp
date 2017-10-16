#include "timer.h"

#include "screen.h"
#include "const.h"
#include "game.h"
#include "logger.h"

namespace timer {
  namespace {
    const int INTERVAL = 100;

    unsigned long nextRun = millis();

    String formatMillis(uint32_t milliSeconds) {
      return String(milliSeconds);
    }

    void updateDisplay() {
      logger::log(logger::TYPE_DEBUG, "clock", "clock thread called");
      screen::display(formatMillis(game::getRemainingTime()));
      nextRun = millis() + INTERVAL;
    }
  }

  void checkUpdateDisplay(){
    if (millis() > nextRun){
      updateDisplay();
    }
  }
}
