#include "../ino_libs/ino_libs.h"
#include "central_controller.h"

// initializeModules performs initialization of structures for the vehicle to operate.
// minDistance is minimal distance between car and object in front,
// to be able to evade collision, imperatively calculated.
// The function sets initial vehicle mode to NONE.
void initializeModules(uint8_t minDistance) {
    // initialize microcontroller
    initPWMTimers();
    initADC();

    // initialize other modules
    _controllerInitJoystick();
    _controllerInitSerial();
    _controllerInitEngines();
    _controllerInitDistanceSensor(minDistance);
    // _controllerInitIRSensor();
    _controllerInitOLED();
    _controllerInitJoystick();
    _controllerInitSpeedSensor();
}
