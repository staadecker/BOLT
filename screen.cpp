#include "screen.h"

#include "logger.h"
#include "const.h"

namespace screen {
  void display(String message) {
    logger::log(logger::TYPE_INFO, "screen", "Displaying : \"" + message + "\"");
    if (constants::IS_SCREEN_CONNECTED) {
      logger::log(logger::TYPE_WARNING, "screen", "Method incomplete");
    }
  }
}
