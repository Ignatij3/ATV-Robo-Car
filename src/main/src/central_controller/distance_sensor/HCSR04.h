#ifndef HCSR04_H
#define HCSR04_H

#include <stdint.h>

void registerDistanceSensor(uint8_t triggerPin, uint8_t echoPin,
                            uint16_t maxTimeoutMicroSec);
uint8_t measureDistanceCm(void);

#endif
