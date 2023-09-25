#ifndef ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#define ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#include <Arduino.h>

#define RX_BUFFER_SIZE 128

#ifdef __cplusplus
extern "C" {
#endif
void serialInit(uint32_t ubrr);
void writeByte(uint8_t c);
void writeArray(uint8_t *c, uint16_t len);
uint8_t readByte(void);
uint16_t readCount(void);
static void enableLED(void);
static void disableLED(void);
#ifdef __cplusplus
}
#endif

#endif