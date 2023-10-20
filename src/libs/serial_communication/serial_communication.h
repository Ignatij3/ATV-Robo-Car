#ifndef ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#define ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#include <stdint.h>

#define NEWLINE "\n\r"

void serialInit(uint32_t ubrr);
void writeUint(uint16_t n);
void writeBinary(uint16_t n);
void writeByte(const char c);
void writeString(const char *str);
void writeStringF(const char *format, ...);
void writeStringLn(const char *str);
char readByte(void);
uint16_t readCount(void);
void uint8Printing(uint8_t n);
void intToStr(uint8_t num, char *str);

#endif