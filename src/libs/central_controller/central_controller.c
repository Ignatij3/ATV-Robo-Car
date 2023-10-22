#include "central_controller.h"

// initializeModules performs initialization of structures for the vehicle to operate.
// minDistance is minimal distance between car and object in front,
// to be able to evade collision, imperatively calculated.
// The function sets initial vehicle mode to NONE.
void initializeModules(uint8_t minDistance, drivingMode drMode) {
    _controllerInitJoystick(drMode);
    _controllerInitSerial();
    _controllerInitEngines();
    _controllerInitDistanceSensor(minDistance);
    _controllerInitIRSensor();
    _controllerInitOLED();
}
