#include "libs/central_controller/central_controller.h"
#include <util/delay.h>

// arbitrary constants, subject to change
#define COLLISION_DISTANCE 20
#define ACCELERATION_RATE 20
#define ENGINE_ADJUSTMENT 40
#define REFRESH_RATE 100

int main(void) {
    initializeModules();
    updateMode();
    enableCar();

    while (1) {
        // halt while car is turned off
        if (!isPoweredOn()) {
            disableCar();
            while (!isPoweredOn()) {
                updateCarMetrics();
            }
            enableCar();
        }

        // halt while user chooses driving mode
        if (joystickPressed()) {
            disableCar();
            updateMode();
            enableCar();
        }

        switch (getMode()) {
        // in automatic mode, car drives forward until colliding
        // afterwards, it turns around and continues forward
        case AUTOMATIC:
            accelerate(ACCELERATION_RATE);
            if (isCollisionSoon(COLLISION_DISTANCE)) {
                evadeCollision(COLLISION_DISTANCE);
            }
            break;

        // in controlled mode, car receives and executes commands from DualShock PS4 controller
        case CONTROLLED:
            readExecuteCommand();
            break;

        // in slave mode, car follows black line. If there is predecessor on a line, the car tailgates it
        case SLAVE:
            accelerate(ACCELERATION_RATE);
            if (isCollisionSoon(COLLISION_DISTANCE)) {
                decelerate(ACCELERATION_RATE);
            }
            updateLinePosition();
            adjustEnginesSpeed(ENGINE_ADJUSTMENT);
            break;

        // if NONE mode is chosen, the car must halt
        case NONE:
            goto exit;

        default:
            break;
        }

        // calculating delay to take in milliseconds
        _delay_ms(1000 / REFRESH_RATE);
    }

exit:
    disableCar();
    return 1;
}