#include "flasher.h"

#include <Thread.h>
#include "controller.h"
#include "logger.h"
#include "led.h"

Thread flasher_thread;

void flasher_callback() {
  logger(LOGGER_TYPE_DEBUG, "flasher", "flasher thread called");
  if (led_flashingCounter > 0) {
    led_update();
  }
}

void flasher_setup(int flashTime) {
  flasher_thread = Thread();
  flasher_thread.setInterval(flashTime);
  flasher_thread.ThreadName = "Flasher";
  flasher_thread.onRun(flasher_callback);
  flasher_thread.enabled = true;
  logger(LOGGER_TYPE_DEBUG, "flasher", "Success : " + String(controller_add(&flasher_thread)));
}
