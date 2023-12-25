#include "../distance_sensor/HCSR04.h"
#include "../engine_controller/engine_controller.h"
#include "central_controller.h"

// _controllerInitDistanceSensor initializes ultrasonic distance sensor.
void _controllerInitDistanceSensor(void) {
    registerDistanceSensor();
}

// isCollision returns whether vehicle is about to collide with object in front.
// This is done by comparing closest object distance with minimal tolerable distance (minDist).
bool isCollisionSoon(uint8_t minDist) {
    return measureDistanceCm() <= minDist;
}

// evadeCollision will tank turn clockwise until there is no objects in front of the car.
// minDist is distance to the object in front of the car at which car will start evading collision.
// During collision evasion, minDist is twice the passed value.
void evadeCollision(uint8_t minDist) {
    if (isCollisionSoon(minDist)) {
        minDist <<= 1;
        tankTurn(isCollisionSoon, (measureDistanceCm() % 2) - 1);
        minDist >>= 1;
    }
}
