#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

const byte LOGGER_TYPE_ERROR = 0;
const byte LOGGER_TYPE_WARNING = 1;
const byte LOGGER_TYPE_INFO = 2;
const byte LOGGER_TYPE_DEBUG = 3;

const byte LOGGER_MODE = LOGGER_TYPE_INFO;

void logger(byte type, String fileName, String message);


#endif
