#ifndef ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#define ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#include <Arduino.h>

void writeSerial(const char* data);
const char* readSerial();
char isUsed();

#endif