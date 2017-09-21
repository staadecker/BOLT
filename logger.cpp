#include "logger.h"

void logger(byte type, String fileName, String message) {
  if (type <= LOGGER_MODE) {
    String typeName;

    switch (type) {
      case LOGGER_TYPE_ERROR:
        typeName = "ERROR";
        break;
      case LOGGER_TYPE_WARNING:
        typeName = "WARNING";
        break;
      case LOGGER_TYPE_DEBUG:
        typeName = "DEBUG";
        break;
      case LOGGER_TYPE_INFO:
        typeName = "INFO";
        break;

      default:
        logger(LOGGER_TYPE_ERROR, "test", "Unkown log type");
    }

    Serial.println(typeName + " : " + fileName + " : " + message + ".");
  }
}

