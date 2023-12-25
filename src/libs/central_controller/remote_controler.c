#include "../remote_control/remote_control.h"
#include "../serial_communication/serial_communication.h"
#include "central_controller.h"

// _controllerInitSerial will initialize UART communication.
void _controllerInitSerial(void) {
    serialInit(DEFAULT_SERIAL_BAUD);
}

void _controllerInitRemote(void) {
    initModule();
}

// readExecuteCommand will read command entered into serial interface, then update engines to reflect command.
void readExecuteCommand(void) {
    readMovementCommand();
    setCarDirection();
}