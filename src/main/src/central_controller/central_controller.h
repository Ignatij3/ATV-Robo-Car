#ifndef ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#define ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SLAVE, CONTROLLED, AUTOMATIC, HALT } drivingMode;

void initializeVehicleControls(uint8_t minimalTolerableDistance);
static bool isCollisionSoon(void);
static void evadeCollision(void);
void setMode(drivingMode mode);
void setActivationThreshold(uint8_t t);
void setControllerTravel(uint16_t minX, uint16_t maxX, uint16_t minY,
                         uint16_t maxY);
void controllerState(uint8_t x, uint8_t y);
#ifdef __cplusplus
}
#endif

#endif