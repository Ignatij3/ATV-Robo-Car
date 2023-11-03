#ifndef ATV_REMOTE_CONTROL_REMOTE_CONTROL_H
#define ATV_REMOTE_CONTROL_REMOTE_CONTROL_H

#include <stdbool.h>
#include <stdint.h>

bool pairWithModule(void);
void readMovementVector(void);
void vectorToEngineSpeed(uint8_t *left, uint8_t *right);

#endif // ATV_REMOTE_CONTROL_REMOTE_CONTROL_H
