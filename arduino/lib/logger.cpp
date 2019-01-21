
#include "logger.h"

const byte MODE = TYPE_INFO;

void log(const byte &type, const String &fileName, const String &message) {
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
                log(TYPE_ERROR, "test", "Unknown log type");
        }

        Serial.println(typeName + " : " + fileName + " : " + message + ".");
    }
}