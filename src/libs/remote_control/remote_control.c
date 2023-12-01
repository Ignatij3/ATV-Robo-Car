#include "remote_control.h"
#include "../engine_controller/engine_controller.h"
#include "../serial_communication/serial_communication.h"
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#define COMMAND_COOLDOWN_MS 1200

typedef struct {
    uint8_t direction;
} moveVector;

static volatile moveVector vec;

// initModule will initialize variables to start listening for commands.
void initModule(void) {
    writeString("AT");
    _delay_ms(COMMAND_COOLDOWN_MS);
    writeString("AT+NAMEcar_controller");
    _delay_ms(COMMAND_COOLDOWN_MS);
    writeString("AT+PIN7800");
    _delay_ms(COMMAND_COOLDOWN_MS);
    vec.direction = '\0';
}

void _OLD_initModule(void) {
    char *s = malloc(sizeof(char) * 10);

    writeString("AT");
    _delay_ms(COMMAND_COOLDOWN_MS);
    while (readCount() < 2)
        ;
    readNBytes(s, 2);

    writeString("AT+NAMEcar_controller");
    _delay_ms(COMMAND_COOLDOWN_MS);
    while (readCount() < 6)
        ;
    readNBytes(s, 9);

    writeString("AT+PIN7400");
    _delay_ms(COMMAND_COOLDOWN_MS);
    while (readCount() < 5)
        ;
    readNBytes(s, 8);

    vec.direction = ' ';
    free(s);
}

// readMovementCommand will read byte from serial indicating what direction is to drive.
void readMovementCommand(void) {
    uint8_t newDir = '0';
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
    } else if (vec.direction == ' ') {
        setSpeed(0, isReverse());
    }
    increaseSpeed(10);
}