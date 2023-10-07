#include "../engine_controller/engine_controller.h"
#include "../global_constants/global_constants.h"
#include "../ino_libs/ino_libs.h"
#include "../serial_communication/serial_communication.h"
#include "central_controller.h"
#include "distance_sensor/HCSR04.h"
#include <util/delay.h>

#define BAUD 9600
#define MYUBRR(baud) F_CPU / 16 / baud - 1

typedef struct {
    uint8_t minimalTolerableDistance;
    drivingMode mode;
    uint8_t speed;
    bool poweredOn;
} carControls;

static carControls vehicle;

static bool isCollisionSoon(void);
static void evadeCollision(void);
static drivingMode readNewMode(void);
static void accelerate(uint8_t step);
static void decelerate(uint8_t step);

// initializeModules performs initialization of structures for the vehicle to operate.
// minimalTolerableDistance is minimal distance between car and object in front,
// to be able to evade collision, imperatively calculated.
void initializeModules(uint8_t minimalTolerableDistance) {
    vehicle.minimalTolerableDistance = minimalTolerableDistance;
    vehicle.mode = NONE;
    vehicle.speed = 0;
    vehicle.poweredOn = false;

    // initialize other modules
    initPWMTimers();
    enablePWM();
    initializeEngines();
    registerDistanceSensor();
    serialInit(MYUBRR(BAUD));
}

// togglePower turns on or off car's controls and returns current state.
bool togglePower(void) {
    vehicle.poweredOn = (vehicle.poweredOn - 1) & 1;
    return vehicle.poweredOn;
}

// isCollision returns whether vehicle is about to collide with object in front.
// This is done by comparing closest object distance with minimal tolerable distance.
static bool isCollisionSoon(void) {
    return measureDistanceCm() < vehicle.minimalTolerableDistance;
}

// evadeCollision will tank turn clockwise until there is no objects in front of the car.
static void evadeCollision(void) {
    setSpeed(0, false);
    vehicle.speed = 0;
    turnRight(isCollisionSoon);
}

// setMode sets passed driving mode.
// If driving mode is NONE, car wouldn't move.
void setMode(drivingMode mode) {
    if (mode != NONE) {
        vehicle.mode = mode;
    }
}

// readNewMode reads signal from joystick that is it pushed.
// It returns selected driving mode.
static drivingMode readNewMode(void) {
    return AUTOMATIC;
}

// accelerate will increase speed of the car by 'step'.
// Once speed is at allowed maximum, accelerate does not change it.
static void accelerate(uint8_t step) {
    if (vehicle.speed == MAX_SPEED) {
        return;
    }

    if (vehicle.speed < MAX_SPEED - step) {
        vehicle.speed += step;
    } else {
        vehicle.speed = MAX_SPEED;
    }
    increaseSpeed(step);
}

// decelerate will decrease speed of the car by 'step'.
// Once speed is 0, decelerate does not change it.
static void decelerate(uint8_t step) {
    if (vehicle.speed == 0) {
        return;
    }

    if (vehicle.speed > step) {
        vehicle.speed -= step;
    } else {
        vehicle.speed = 0;
    }
    decreaseSpeed(step);
}

// run drives the car in according to driving mode, blocks on execution.
// If mode is set to "NONE" or vehicle power is switched off, the function exits.
void run(void) {
    setTorqueDirection();
    while (vehicle.poweredOn) {
        // update driving mode
        vehicle.mode = readNewMode();

        switch (vehicle.mode) {
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
            /* code */
            break;

        // in slave mode, car follows black line. If there is predecessor on a line, the car tailgates it
        case SLAVE:
            /* code */
            break;

        // if NONE mode is chosen, the car must halt
        case NONE:
            goto exit;
        }
        // delay not to change state too rapidly
        _delay_ms(10);
    }

exit:
    setSpeed(0, false);
    vehicle.speed = 0;
    turnOffEngines();
}
