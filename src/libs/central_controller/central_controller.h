#ifndef ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#define ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#include <stdbool.h>
#include <stdint.h>

typedef enum { NONE, SLAVE, CONTROLLED, AUTOMATIC } drivingMode;

void initializeModules(uint8_t minimalTolerableDistance);
bool togglePower(void);
void setMode(drivingMode mode);
void run(void);

#endif