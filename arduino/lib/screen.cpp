#include <string.h>
#include <HardwareSerial.h>
#include "screen.h"

namespace screen {
    void display(const char *message) {
        Serial.print("Displaying: ");
        Serial.println(message);
    }
}
