#ifndef ATV_ENGINE_CONTROLLER_ENGINE_CONTROLLER_H
#define ATV_ENGINE_CONTROLLER_ENGINE_CONTROLLER_H
#include <stdbool.h>
#include <stdint.h>

void initializeEngines(void);
void setTorqueDirection(void);
void turnOffEngines(void);
void turnLeft(bool (*cancelFunc)(void));
void turnRight(bool (*cancelFunc)(void));
void setSpeed(uint8_t speed, bool reverse);
void increaseSpeed(uint8_t speed);
void decreaseSpeed(uint8_t speed);

#endif