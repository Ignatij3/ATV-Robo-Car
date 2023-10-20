#ifndef ATV_IR_SENSOR_IR_SENSOR_H
#define ATV_IR_SENSOR_IR_SENSOR_H
#include <stdbool.h>

void initializeIR(void);
void updateIRReadings(void);
bool isOnLine(void);
bool isOffLine(void);
bool isOnLeftOfLine(void);
bool isOnRightOfLine(void);

#endif // ATV_IR_SENSOR_IR_SENSOR_H