#include "distance_sensor_controls.h"
#include <stdint.h>

extern uint8_t minDist;

// isCollision returns whether vehicle is about to collide with object in front.
// This is done by comparing closest object distance with minimal tolerable distance.
bool isCollisionSoon(void) {
    return measureDistanceCm() < minDist;
}

// evadeCollision will tank turn clockwise until there is no objects in front of the car.
void evadeCollision(void) {
    setSpeed(0, false);
    turnRight(isCollisionSoon);
}
