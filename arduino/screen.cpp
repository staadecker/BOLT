#include "screen.h"

#include "logger.h"
#include "constants.h"

namespace screen {
void display(String message) {
  logger::log(logger::TYPE_INFO, "screen", "Displaying : \"" + message + "\"");
}
}
