#ifndef ATV_ENGINE_CONTROLLER_ENGINE_CONTROLLER_H
#define ATV_ENGINE_CONTROLLER_ENGINE_CONTROLLER_H
#include <stdbool.h>
#include <stdint.h>

void initializeEngines(void);
void setEnginesDirection(bool reverse);
void turnOffEngines(void);
void enableTurning(int8_t direction);
void tankTurn(bool (*cancelFunc)(void), int8_t direction);
void setLeftSpeed(uint8_t speed, bool reverse);
void setRightSpeed(uint8_t speed, bool reverse);
void setSpeed(uint8_t speed, bool reverse);
void increaseSpeed(uint8_t speed);
void decreaseSpeed(uint8_t speed);
uint8_t getSpeed(void);
uint8_t getLeftSpeed(void);
uint8_t getRightSpeed(void);
bool isReverse(void);

void TESTING(void);

#endif // ATV_ENGINE_CONTROLLER_ENGINE_CONTROLLER_H