#ifndef ATV_ENGINE_CONTROLLER_ENGINE_CONTROLLER_H
#define ATV_ENGINE_CONTROLLER_ENGINE_CONTROLLER_H
#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
void initializeEngines(void);
void goForward(void);
void turnLeft(uint16_t angle);
void turnRight(uint16_t angle);
void turnAround(void);
void setSpeed(uint8_t speed, bool reverse);
void increaseSpeed(uint8_t speed);
void decreaseSpeed(uint8_t speed);
#ifdef __cplusplus
}
#endif

#endif