#include "../distance_sensor/HCSR04.h"
#include "../engine_controller/engine_controller.h"
#include "central_controller.h"

static uint8_t minDist;

// _controllerInitDistanceSensor initializes ultrasonic distance sensor.
void _controllerInitDistanceSensor(uint8_t minimalDistance) {
    minDist = minimalDistance;
    registerDistanceSensor();
}

// isCollision returns whether vehicle is about to collide with object in front.
// This is done by comparing closest object distance with minimal tolerable distance.
bool isCollisionSoon(void) {
    return measureDistanceCm() < minDist;
}

// evadeCollision will tank turn clockwise until there is no objects in front of the car.
void evadeCollision(void) {
    setSpeed(0, isReverse());
    tankTurnRight(isCollisionSoon);
}
