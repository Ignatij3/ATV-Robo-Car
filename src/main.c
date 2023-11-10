#include "libs/central_controller/central_controller.h"
#include <util/delay.h>

int main(void) {
    initializeModules(20);
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
            accelerate(20);
            if (isCollisionSoon()) {
                evadeCollision();
            }
            break;

        // in controlled mode, car receives and executes commands from DualShock PS4 controller
        case CONTROLLED:
            readExecuteCommand();
            break;

        // in slave mode, car follows black line. If there is predecessor on a line, the car tailgates it
        case SLAVE:
            accelerate(20);
            updateLinePosition();
            adjustEnginesSpeed(30); // 30 - arbitrary constant, subject to change
            break;

        // if NONE mode is chosen, the car must halt
        case NONE:
            goto exit;

        default:
            break;
        }
        // delay not to change state too rapidly
        _delay_ms(10);
    }

exit:
    disableCar();
    return 1;
}