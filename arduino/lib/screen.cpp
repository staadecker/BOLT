#include <string.h>
#include <HardwareSerial.h>
#include "screen.h"

namespace screen {
    void display(const char *message) {
        Serial.print(F("Displaying: "));
        Serial.println(message);
    }
}
