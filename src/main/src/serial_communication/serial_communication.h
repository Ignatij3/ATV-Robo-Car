#ifndef ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#define ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif
void serialInit(unsigned int ubrr);
void writeSerial(const char *data);
const char *readSerial();
static void enableLED(void);
static void disableLED(void);
#ifdef __cplusplus
}
#endif

#endif