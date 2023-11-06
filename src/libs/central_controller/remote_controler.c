#include "../engine_controller/engine_controller.h"
#include "../global_constants/global_constants.h"
#include "../remote_control/remote_control.h"
#include "../serial_communication/serial_communication.h"
#include "central_controller.h"

#define MYUBRR(baud) CPU_CLOCK / 16 / baud - 1

// _controllerPairWithBluetooth will try to pair with remote controller communication device.
void _controllerPairWithBluetooth(void) {
    serialInit(MYUBRR(SERIAL_BAUD));
    // while (!pairWithModule())
    //     ;
}

// readAndSetMovementVector will read updated input vector from the module and update engines to reflect it.
void readAndSetMovementVector(void) {
    readMovementVector();

    uint8_t leftEngine, rightEngine;
    vectorToEngineSpeed(&leftEngine, &rightEngine);

    setLeftSpeed(leftEngine, isReverse());
    setRightSpeed(rightEngine, isReverse());
}