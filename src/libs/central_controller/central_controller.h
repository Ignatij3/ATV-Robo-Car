#ifndef ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#define ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#include <stdbool.h>
#include <stdint.h>

typedef enum { NONE, SLAVE, CONTROLLED, AUTOMATIC } drivingMode;

void initializeModules(uint8_t minimalTolerableDistance);
void enableCar(void);
void disableCar(void);
bool isCollisionSoon(void);
void evadeCollision(void);
drivingMode readNewMode(void);
void accelerate(uint8_t step);
void decelerate(uint8_t step);
void setEngineSpeed(uint8_t speed);
void reverseEngines(void);
void setMode(drivingMode mode);
drivingMode getMode(void);

#endif