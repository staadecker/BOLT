#include "screen.h"

#include "logger.h"

void screen_display(String message) {
  logger(LOGGER_TYPE_INFO, "screen", "Displaying : " + message);
  logger(LOGGER_TYPE_WARNING, "screen", "Method incomplete");
  
}
