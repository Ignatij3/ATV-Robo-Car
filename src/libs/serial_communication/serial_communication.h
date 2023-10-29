#ifndef ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#define ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#include <stdint.h>

#define NEWLINE "\r\n"
#define SERIAL_BAUD 9600 // communication speed

void serialInit(uint32_t ubrr);
void writeUint(uint32_t n);
void writeBinary(uint32_t n);
void writeByte(const char c);
void writeString(const char *str);
void writeStringF(const char *format, ...);
void writeStringLn(const char *str);
char readByte(void);
void readNBytes(uint16_t n, char *str);
void readLine(char *str);
void readUntil(const char c, char *str);
uint16_t readCount(void);

#endif // ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H