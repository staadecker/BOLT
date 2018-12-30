#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

namespace logger {
const byte TYPE_ERROR = 0;
const byte TYPE_WARNING = 1;
const byte TYPE_INFO = 2;
const byte TYPE_DEBUG = 3;

const byte MODE = TYPE_INFO;

void log(byte type, String fileName, String message);
}

#endif
