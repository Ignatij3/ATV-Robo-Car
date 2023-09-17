#include "../constants/constants.h"
#include "../engine_controller/engine_controller.h"
#include "central_controller.h"
#include "distance_sensor/HCSR04.h"

typedef struct {
    uint8_t minimalTolerableDistance;
    drivingMode mode;
} carControls;

static carControls vehicle;
// initialize performs initialization of structures for the vehicle to operate.
// minimalTolerableDistance is minimal distance between car and object in front,
// to be able to safely evade collision.
void initializeVehicleControls(
    uint8_t minimalTolerableDistance /*, inputs SERIAL_INPUTS*/) {
    initializeEngines();
    registerDistanceSensor(DISTANCE_SENSOR_TRIGGER_PIN,
                           DISTANCE_SENSOR_ECHO_PIN, 0);
}

// isCollision returns whether vehicle is about to collide with object in front.
static bool isCollisionSoon(void) {
    uint8_t distance = measureDistanceCm();
    return distance < vehicle.minimalTolerableDistance;
}

// evadeCollision will tank turn clockwise until there is no object in front of
// the car.
static void evadeCollision(void) {
    setSpeed(0, false);
    do {
        turnRight(1);
    } while (isCollisionSoon());
}

// setMode sets specified driving mode.
void setMode(drivingMode mode) { vehicle.mode = mode; }

void setActivationThreshold(uint8_t t) {}

void setControllerTravel(uint16_t minX, uint16_t maxX, uint16_t minY,
                         uint16_t maxY) {}

void controllerState(uint8_t x, uint8_t y) {}

void run() {}