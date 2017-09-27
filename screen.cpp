#include "screen.h"

#include "logger.h"
#include "const.h"

void screen_display(String message) {
  logger(LOGGER_TYPE_INFO, "screen", "Displaying : " + message);
  if (IS_SCREEN_CONNECTED) {
    logger(LOGGER_TYPE_WARNING, "screen", "Method incomplete");
  }
}
