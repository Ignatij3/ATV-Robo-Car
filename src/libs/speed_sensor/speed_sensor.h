#ifndef SPEED_SENSOR
#include <avr/io.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void registerSpeedSensor(void);
long long getNanoTime(void);
void updateSpeed(void);

#endif
