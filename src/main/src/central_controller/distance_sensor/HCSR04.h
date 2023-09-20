#ifndef HCSR04_H
#define HCSR04_H

#include <stdint.h>

void registerDistanceSensor(uint16_t maxTimeoutMicroSec);
uint8_t measureDistanceCm(void);

#endif
