#ifndef ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#define ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#include <stdint.h>

void serialInit(uint32_t ubrr);
void writeByte(uint8_t c);
void writeArray(uint8_t *c, uint16_t len);
void writeString(uint8_t *c);
uint8_t readByte(void);
uint16_t readCount(void);

#endif