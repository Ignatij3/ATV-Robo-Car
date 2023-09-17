#ifndef ATV_ENGINE_CONTROLLER_ENGINE_CONTROLLER_H
#define ATV_ENGINE_CONTROLLER_ENGINE_CONTROLLER_H
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif
void turnLeft(uint16_t angle);
void turnRight(uint16_t angle);
void turnAround(uint16_t angle);
void setSpeed(uint8_t speed, bool reverse);
void increaseSpeed(uint8_t speed);
void decreaseSpeed(uint8_t speed);
#ifdef __cplusplus
}
#endif
#endif