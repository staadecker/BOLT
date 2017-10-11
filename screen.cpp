#include "screen.h"

#include "logger.h"
#include "const.h"

namespace screen{
void displayToScreen(String message) {
  logger::logger(logger::TYPE_INFO, "screen", "Displaying : \"" + message + "\"");
  if (constants::IS_SCREEN_CONNECTED) {
    logger::logger(logger::TYPE_WARNING, "screen", "Method incomplete");
  }
}}
