#ifndef HCSR04_H
#define HCSR04_H

#include <stdint.h>

void registerDistanceSensor(void);
uint8_t measureDistanceCm(void);
void sendPulse();

#endif
