#include "flasher.h"

#include <Thread.h>
#include "controller.h"
#include "logger.h"
#include "led.h"

bool flashingCurrentState = true;
bool flashed = false;

Thread flasher_thread = Thread();

void flasher_callback() {
  logger(LOGGER_TYPE_DEBUG, "flasher", "flasher thread called");

  //Variable should turn true if something flahed
  flashed = false;

  led_shiftOut();

  if (!flashed) {
    //Nothing is flashing anymore
    flasher_thread.enabled = false;
  } else {
    //If it did flash switch flashing state
    flashingCurrentState = !flashingCurrentState;
  }
}

void flasher_setup() {
  flasher_thread.setInterval(FLASHER_INTERVAL);
  flasher_thread.onRun(flasher_callback);
  flasher_thread.enabled = false;
  flasher_thread.ThreadName = "flasher";
  controller_add(&flasher_thread);
}
