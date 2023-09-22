#ifndef ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#define ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif
void serialInit(uint8_t ubrr);
void writeSerial(const char *data);
const char *readSerial();
bool isUsed();
#ifdef __cplusplus
}
#endif

#endif