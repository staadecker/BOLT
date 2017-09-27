#include "helper.h"

#include "controller.h"
#include <Arduino.h>

void helper_waitTime(unsigned long milliseconds){
      unsigned long startTime = millis();
    while (millis() - startTime < milliseconds) {
      controller_run();
    }
}
