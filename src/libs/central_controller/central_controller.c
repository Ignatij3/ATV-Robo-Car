#include "../engine_controller/engine_controller.h"
#include "../global_constants/global_constants.h"
#include "../ino_libs/ino_libs.h"
#include "../serial_communication/serial_communication.h"
#include "central_controller.h"
#include "distance_sensor/HCSR04.h"

#define MYUBRR(baud) F_CPU / 16 / baud - 1

typedef struct {
    uint8_t minimalTolerableDistance;
    drivingMode mode;
    uint8_t speed;
    bool reverse;
} carControls;

static carControls vehicle;

// initializeModules performs initialization of structures for the vehicle to operate.
// minimalTolerableDistance is minimal distance between car and object in front,
// to be able to evade collision, imperatively calculated.
// The function sets initial vehicle mode to NONE.
void initializeModules(uint8_t minimalTolerableDistance) {
    vehicle.minimalTolerableDistance = minimalTolerableDistance;
    vehicle.mode = NONE;
    vehicle.speed = 0;

    // initialize other modules
    initPWMTimers();
    enablePWM();
    initializeEngines();
    registerDistanceSensor();
    serialInit(MYUBRR(BAUD));
}

// enableCar enables cars engines.
// Function configures engines to run forward.
void enableCar(void) {
    turnOnEngines(false);
    setSpeed(0, false);
    vehicle.speed = 0;
}

// disableCar halts the car, disabling it's engines.
// To continue driving, call 'enableCar'.
void disableCar(void) {
    setSpeed(0, false);
    vehicle.speed = 0;
    turnOffEngines();
}

// isCollision returns whether vehicle is about to collide with object in front.
// This is done by comparing closest object distance with minimal tolerable distance.
bool isCollisionSoon(void) {
    return measureDistanceCm() < vehicle.minimalTolerableDistance;
}

// evadeCollision will tank turn clockwise until there is no objects in front of the car.
void evadeCollision(void) {
    setSpeed(0, false);
    vehicle.speed = 0;
    turnRight(isCollisionSoon);
}

// readNewMode reads signal from joystick that is it pushed.
// It returns selected driving mode.
drivingMode readNewMode(void) {
    return AUTOMATIC;
}

// accelerate will increase speed of the car by 'step'.
// Once speed is at allowed maximum, accelerate does not change it.
void accelerate(uint8_t step) {
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
void decelerate(uint8_t step) {
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

// setEngineSpeed sets engine duty cycle proportional to fraction speed/255.
void setEngineSpeed(uint8_t speed) {
    setSpeed(speed, vehicle.reverse);
}

// reverseEngines reverses engine torque direction.
void reverseEngines(void) {
    vehicle.reverse = !vehicle.reverse;
    setSpeed(vehicle.speed, vehicle.reverse);
}

// setMode sets passed driving mode.
// If driving mode is NONE, setMode wouldn't accept it.
void setMode(drivingMode mode) {
    if (mode != NONE) {
        vehicle.mode = mode;
    }
}

// getMode returns driving mode.
drivingMode getMode(void) {
    return vehicle.mode;
}