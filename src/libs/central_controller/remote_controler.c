#include "../engine_controller/engine_controller.h"
#include "../remote_control/remote_control.h"
#include "central_controller.h"

// _controllerPairWithBluetooth will try to pair with remote controller communication device.
void _controllerPairWithBluetooth(void) {
    while (!pairWithModule())
        ;
}

// readAndSetMovementVector will read updated input vector from the module and update engines to reflect it.
void readAndSetMovementVector(void) {
    readMovementVector();

    uint8_t leftEngine, rightEngine;
    vectorToEngineSpeed(&leftEngine, &rightEngine);

    setLeftSpeed(leftEngine, isReverse());
    setRightSpeed(rightEngine, isReverse());
}