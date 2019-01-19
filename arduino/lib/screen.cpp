#include "screen.h"

#include "logger.h"

namespace screen {
    void display(String message) {
        log(TYPE_INFO, "screen", "Displaying : \"" + message + "\"");
    }
}
