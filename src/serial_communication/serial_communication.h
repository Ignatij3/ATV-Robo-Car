#ifndef ATV_SERIAL_COMMUNICATION_SERIAL_COMMUNICATION_H
#define ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H

void writeSerial(const char* data);
const char* readSerial();
char isUsed();

#endif