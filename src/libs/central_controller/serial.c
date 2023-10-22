#include "../global_constants/global_constants.h"
#include "../serial_communication/serial_communication.h"
#include "central_controller.h"

#define MYUBRR(baud) CPU_CLOCK / 16 / baud - 1

// _controllerInitSerial initializes serial communication.
void _controllerInitSerial(void) {
    serialInit(MYUBRR(SERIAL_BAUD));
}