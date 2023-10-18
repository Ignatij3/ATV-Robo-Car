#include "../engine_controller/engine_controller.h"
#include "../global_constants/global_constants.h"
#include "../ino_libs/ino_libs.h"
#include "../serial_communication/serial_communication.h"
#include "central_controller.h"
#include "distance_sensor/HCSR04.h"

#define MYUBRR(baud) CPU_CLOCK / 16 / baud - 1

static uint8_t minimalTolerableDistance;
static drivingMode mode;

// initializeModules performs initialization of structures for the vehicle to operate.
// minimalTolerableDistance is minimal distance between car and object in front,
// to be able to evade collision, imperatively calculated.
// The function sets initial vehicle mode to NONE.
void initializeModules(uint8_t minimalTolerableDistance) {
    minimalTolerableDistance = minimalTolerableDistance;
    mode = NONE;

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
    setEnginesDirection(true);
    setSpeed(0, false);
}

// disableCar halts the car, disabling it's engines.
// To continue driving, call 'enableCar'.
void disableCar(void) {
    setSpeed(0, false);
    turnOffEngines();
}

// isCollision returns whether vehicle is about to collide with object in front.
// This is done by comparing closest object distance with minimal tolerable distance.
bool isCollisionSoon(void) {
    return measureDistanceCm() < minimalTolerableDistance;
}

// evadeCollision will tank turn clockwise until there is no objects in front of the car.
void evadeCollision(void) {
    setSpeed(0, false);
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
    increaseSpeed(step);
}

// decelerate will decrease speed of the car by 'step'.
// Once speed is 0, decelerate does not change it.
void decelerate(uint8_t step) {
    decreaseSpeed(step);
}

// setEngineSpeed sets engine duty cycle proportional to fraction speed/255.
void setEngineSpeed(uint8_t speed) {
    setSpeed(speed, isReverse());
}

// reverseEngines reverses engine torque direction.
void reverseEngines(void) {
    setSpeed(getSpeed(), !isReverse());
}

// setMode sets passed driving mode.
// If driving mode is NONE, setMode wouldn't accept it.
void setMode(drivingMode newMode) {
    if (newMode != NONE) {
        mode = newMode;
    }
}

// getMode returns driving mode.
drivingMode getMode(void) {
    return mode;
}