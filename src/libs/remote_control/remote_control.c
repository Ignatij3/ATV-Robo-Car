#include "remote_control.h"
#include "../engine_controller/engine_controller.h"
#include "../serial_communication/serial_communication.h"
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

typedef struct {
    uint8_t direction;
} moveVector;

static volatile moveVector vec;

// initModule will initialize variables to start listening for commands.
void initModule(void) {
    vec.direction = '\0';
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
    if (getSpeed() < 100) {
        setSpeed(105, isReverse());
    }
    increaseSpeed(5);

    if (vec.direction == 'w' || vec.direction == 's') {
        setEnginesDirection(vec.direction == 's');
    } else if (vec.direction == 'a' || vec.direction == 'd') {
        enableTurning((vec.direction == 'd') - (vec.direction == 'a'));
    } else if (vec.direction == ' ') {
        setSpeed(0, isReverse());
    }
}