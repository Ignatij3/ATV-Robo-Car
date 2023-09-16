#ifndef ATV_ENGINE_CONTROLLER_ENGINE_CONTROLLER_H
#define ATV_ENGINE_CONTROLLER_ENGINE_CONTROLLER_H
#include <stdint.h>
#include <stdbool.h>

void turnLeft(uint16_t angle);
void turnRight(uint16_t angle);
void turnAround(uint16_t angle);
void setSpeed(uint8_t speed, bool reverse);
void increaseSpeed(uint8_t speed);
void decreaseSpeed(uint8_t speed);

#endif