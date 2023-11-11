#include "../global_constants/global_constants.h"
#include "../remote_control/remote_control.h"
#include "../serial_communication/serial_communication.h"
#include "central_controller.h"

#define MYUBRR(baud) CPU_CLOCK / 16 / baud - 1

static void pairWithBluetooth(void);

// _controllerInitSerial will initialize UART communication.
void _controllerInitSerial(void) {
    serialInit(MYUBRR(SERIAL_BAUD));
}

// pairWithBluetooth will try to pair with remote controller communication device.
static void pairWithBluetooth(void) {
    static bool paired = false;
    if (!paired) {
        if (pairWithModule()) {
            paired = true;
            writeString("\r\ndone\r\n");
        }
    }
}

// readExecuteCommand will read command entered into serial interface, then update engines to reflect command.
void readExecuteCommand(void) {
    pairWithBluetooth();
    readMovementCommand();
    setCarDirection();
}