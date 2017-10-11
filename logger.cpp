#include "logger.h"

namespace logger{
void logger(byte type, String fileName, String message) {
  if (type <= MODE) {
    String typeName;

    switch (type) {
      case TYPE_ERROR:
        typeName = "ERROR";
        break;
      case TYPE_WARNING:
        typeName = "WARNING";
        break;
      case TYPE_DEBUG:
        typeName = "DEBUG";
        break;
      case TYPE_INFO:
        typeName = "INFO";
        break;

      default:
        logger(TYPE_ERROR, "test", "Unkown log type");
    }

    Serial.println(typeName + " : " + fileName + " : " + message + ".");
  }
}
}
