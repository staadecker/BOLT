#include "flasher.h"

#include <Thread.h>
#include "controller.h"
#include "logger.h"
#include "led.h"
#include "const.h"

namespace flasher {
  namespace {
    const int FLASHER_INTERVAL = 1000;

    bool flashing[constants::NUMBER_OF_LEDS];

    bool currentFlashingState = LOW;

    Thread flasher_thread = Thread();

    void callback() {
      logger::log(logger::TYPE_DEBUG, "flasher", "flasher thread called");

      currentFlashingState = !currentFlashingState;

      for (uint8_t led = 0 ; led < constants::NUMBER_OF_LEDS ; led++) {
        if (flashing[led]) {
          if(currentFlashingState){
            led::turnOn(led);
          } else{
            led::turnOff(led);
          }
        }
      }
    }
  }

  void flash(uint8_t ledNumber) {
    flashing[ledNumber] = true;
  }

  void stopFlashing(uint8_t ledNumber) {
    flashing[ledNumber] = false;
    led::turnOff(ledNumber);
  }

  void setup() {
    flasher_thread.setInterval(FLASHER_INTERVAL);
    flasher_thread.onRun(callback);
    flasher_thread.ThreadName = "flasher";
    controller::add(&flasher_thread);
  }
}
