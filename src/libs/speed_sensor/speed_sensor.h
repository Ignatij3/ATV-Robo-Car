#ifndef ATV_SPEED_SENSOR_SPEED_SENSOR_H
#define ATV_SPEED_SENSOR_SPEED_SENSOR_H

#include <avr/interrupt.h>
#include <avr/io.h>

void registerSpeedSensor(void);
uint8_t getSensorSpeedReading(void);

#endif // ATV_SPEED_SENSOR_SPEED_SENSOR_H
