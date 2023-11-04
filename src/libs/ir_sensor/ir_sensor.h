#ifndef ATV_IR_SENSOR_IR_SENSOR_H
#define ATV_IR_SENSOR_IR_SENSOR_H

#include <math.h>

#define UNKNOWN INT8_MIN
#define LINE_TOO_FAR_LEFT INT8_MIN / 2
#define LINE_CENTERED 0
#define LINE_TOO_FAR_RIGHT INT8_MAX / 2
#define ALL INT8_MAX

void initializeIR(void);
int8_t updateIRReadings(void);
int8_t maxDeviation(void);
int8_t minDeviation(void);

#endif // ATV_IR_SENSOR_IR_SENSOR_H