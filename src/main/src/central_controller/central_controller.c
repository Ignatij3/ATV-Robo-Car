#include "central_controller.h"
#include "../constants/constants.h"

void initTEST() {
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, HIGH);
}

bool togglePowerState()
{
}

void setDistanceFromObject(uint8_t distance)
{
}

void setMode(drivingMode mode)
{
}

void setActivationThreshold(uint8_t t)
{
}

void setControllerTravel(uint16_t minX, uint16_t maxX, uint16_t minY, uint16_t maxY)
{
}

void controllerState(uint8_t x, uint8_t y)
{
}
