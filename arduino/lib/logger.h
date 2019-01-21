#ifndef LOGGER_H
#define LOGGER_H


#include <USBAPI.h>

const byte TYPE_ERROR = 0;
const byte TYPE_WARNING = 1;
const byte TYPE_INFO = 2;
const byte TYPE_DEBUG = 3;


void log(const byte &type, const String &fileName, const String &message);


#endif
