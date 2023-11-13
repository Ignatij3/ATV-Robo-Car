#include "../engine_controller/engine_controller.h"
#include "../serial_communication/serial_communication.h"
#include "remote_control.h"
#include <string.h>

typedef struct {
    uint8_t direction;
} moveVector;

static volatile moveVector vec;

// initModule will initialize variables to start listening for commands.
void initModule(void) {
    vec.direction = ' ';
}

// readMovementCommand will read byte from serial indicating what direction is to drive.
void readMovementCommand(void) {
    uint8_t newDir = 0;
    if (readCount() > 0) {
        newDir = readByte();
    }
    if (newDir == 'w' || newDir == 'a' || newDir == 's' || newDir == 'd' || newDir == ' ') {
        vec.direction = newDir;
    }
}

// setCarDirection will set direction of movement based on the last obtained command.
void setCarDirection(void) {
    if (vec.direction == 'w' || vec.direction == 's') {
        setEnginesDirection(vec.direction == 's');
    } else if (vec.direction == 'a' || vec.direction == 'd') {
        enableTurning((vec.direction == 'd') - (vec.direction == 'a'));
    }

    setSpeed(255, isReverse());
    if (vec.direction == ' ') {
        setSpeed(0, isReverse());
    }
}