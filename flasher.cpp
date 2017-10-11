#include "flasher.h"

#include <Thread.h>
#include "controller.h"
#include "logger.h"
#include "led.h"

namespace flasher {
  namespace {
    const int FLASHER_INTERVAL = 1000;

    Thread flasher_thread = Thread();
    
    void callback() {
      logger::log(logger::TYPE_DEBUG, "flasher", "flasher thread called");
      led::flash();
    }
  }

  void setup() {
    flasher_thread.setInterval(FLASHER_INTERVAL);
    flasher_thread.onRun(callback);
    flasher_thread.ThreadName = "flasher";
    controller::add(&flasher_thread);
  }
}
