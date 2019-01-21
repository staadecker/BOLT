#include "screen.h"
#include "logger.h"


namespace screen {
    void display(const String &message) {
        log(TYPE_INFO, "screen", "Displaying : \"" + message + "\"");
    }
}
