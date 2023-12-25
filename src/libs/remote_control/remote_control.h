#ifndef ATV_REMOTE_CONTROL_REMOTE_CONTROL_H
#define ATV_REMOTE_CONTROL_REMOTE_CONTROL_H

#include <stdbool.h>
#include <stdint.h>

void initModule(void);
void readMovementCommand(void);
void setCarDirection(void);

#endif // ATV_REMOTE_CONTROL_REMOTE_CONTROL_H
