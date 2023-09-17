#ifndef ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#define ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    SLAVE,
    CONTROLLED,
    AUTOMATIC
} drivingMode;

void initTEST();
bool togglePowerState();
void setDistanceFromObject(uint8_t distance);
void setMode(drivingMode mode);
void setActivationThreshold(uint8_t t);
void setControllerTravel(uint16_t minX, uint16_t maxX, uint16_t minY, uint16_t maxY);
void controllerState(uint8_t x, uint8_t y);
#ifdef __cplusplus
}
#endif
#endif