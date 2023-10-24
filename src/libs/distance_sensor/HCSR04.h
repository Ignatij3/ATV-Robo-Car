#ifndef ATV_DISTANCE_SENSOR_HCSR04_H
#define ATV_DISTANCE_SENSOR_HCSR04_H

#include <stdint.h>

void registerDistanceSensor(void);
uint8_t measureDistanceCm(void);
void sendPulse(void);

#endif // ATV_DISTANCE_SENSOR_HCSR04_H
