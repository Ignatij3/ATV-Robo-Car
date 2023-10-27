#include "libs/central_controller/central_controller.h"
#include <util/delay.h>

// update rate of 60Hz
#define INTERVAL_MS 1000 / 60

int main(void) {
    initializeModules(10);
    updateMode();
    enableCar();
    uint32_t previousTime = 0;

    while (1) {
        // halt while car is turned off
        if (!isPoweredOn()) {
            disableCar();
            while (!isPoweredOn()) {
            }
            enableCar();
        }

        if (joystickPressed()) {
            disableCar();
            updateMode();
            enableCar();
        }

        if (millis() - previousTime > INTERVAL_MS) {
            previousTime = millis();
            updateCarMetrics();
        }

        switch (getMode()) {
        // in automatic mode, car drives forward until colliding
        // afterwards, it turns around and continues forward
        case AUTOMATIC:
            accelerate(1);

            // if (isCollisionSoon()) {
            //     evadeCollision();
            // }
            break;

        // in controlled mode, car receives and executes commands from DualShock PS4 controller
        case CONTROLLED:
            /* read commands from serial here */
            break;

        // in slave mode, car follows black line. If there is predecessor on a line, the car tailgates it
        case SLAVE:
            // updateLinePosition(); // holds back
            adjustEnginesSpeed(30); // 30 - arbitrary constant, subject to change
            break;

        // if NONE mode is chosen, the car must halt
        case NONE:
            goto exit;
        }
        // delay not to change state too rapidly
        _delay_ms(10);
    }

exit:
    disableCar();
    return 1;
}