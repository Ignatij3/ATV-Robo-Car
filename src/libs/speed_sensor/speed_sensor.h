#ifndef SPEED_SENSOR
#include <avr/io.h>
#include <time.h>
#include <stdio.h>

void registerSpeedSensor(void);
long long getNanoTime(void);
void updateSpeed(void);

#endif
