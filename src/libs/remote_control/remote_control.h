#ifndef ATV_REMOTE_CONTROL_REMOTE_CONTROL_H
#define ATV_REMOTE_CONTROL_REMOTE_CONTROL_H

#include <stdbool.h>
#include <stdint.h>

bool pairWithModule(void);
void readMovementCommand(void);
void setCarDirection(void);

#endif // ATV_REMOTE_CONTROL_REMOTE_CONTROL_H
