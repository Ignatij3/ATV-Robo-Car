#ifndef ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#define ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef enum { NONE, SLAVE, CONTROLLED, AUTOMATIC } drivingMode;

void initializeVehicleControls(uint8_t minimalTolerableDistance);
static bool isCollisionSoon(void);
static void evadeCollision(void);
void setDistanceFromObject(uint8_t distance);
void setMode(drivingMode mode);
static drivingMode readModeChange(void);
static void accelerate(uint8_t step);
static void decelerate(uint8_t step);
void run(void);
#ifdef __cplusplus
}
#endif

#endif