#include "flasher.h"

#include <Thread.h>
#include "controller.h"
#include "logger.h"
#include "led.h"

namespace flasher {
  Thread flasher_thread = Thread();


  bool flashingCurrentState = true;
  bool flashed = false;

  void callback() {
    logger::logger(logger::TYPE_DEBUG, "flasher", "flasher thread called");

    //Variable should turn true if something flahed
    flashed = false;

    led::shiftOutLed();

    if (!flashed) {
      //Nothing is flashing anymore
      flasher_thread.enabled = false;
    } else {
      //If it did flash switch flashing state
      flashingCurrentState = !flashingCurrentState;
    }
  }

  void setupFlasher() {
    flasher_thread.setInterval(FLASHER_INTERVAL);
    flasher_thread.onRun(callback);
    flasher_thread.enabled = false;
    flasher_thread.ThreadName = "flasher";
    controller::addThread(&flasher_thread);
  }
}
