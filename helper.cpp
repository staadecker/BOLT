#include "helper.h"

#include "controller.h"
#include <Arduino.h>

namespace helper {
  void waitTime(unsigned long milliseconds) {
    unsigned long startTime = millis();
    while (millis() - startTime < milliseconds) {
      controller::runController();
    }
  }
}
