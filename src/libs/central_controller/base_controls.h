#ifndef ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#error "Must include central_controller.h"
#endif // ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H

#ifndef ATV_CENTRAL_CONTROLLER_BASE_CONTROLS_H
#define ATV_CENTRAL_CONTROLLER_BASE_CONTROLS_H
#include <stdint.h>

typedef enum { NONE, SLAVE, CONTROLLED, AUTOMATIC } drivingMode;

void initializeModules(uint8_t minimalTolerableDistance);
drivingMode readNewMode(void);
void setMode(drivingMode mode);
drivingMode getMode(void);

#endif // ATV_CENTRAL_CONTROLLER_BASE_CONTROLS_H