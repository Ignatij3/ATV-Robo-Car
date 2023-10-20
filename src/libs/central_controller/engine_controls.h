#ifndef ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#error "Must include central_controller.h"
#endif // ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H

#ifndef ATV_CENTRAL_CONTROLLER_ENGINE_CONTROLS_H
#define ATV_CENTRAL_CONTROLLER_ENGINE_CONTROLS_H
#include <stdint.h>

void enableCar(void);
void disableCar(void);
void accelerate(uint8_t step);
void decelerate(uint8_t step);
void setEngineSpeed(uint8_t speed);
void reverseEngines(void);

#endif // ATV_CENTRAL_CONTROLLER_ENGINE_CONTROLS_H