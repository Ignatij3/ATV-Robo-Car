#include "speed_sensor.h"
#include "../engine_controller/engine_controller.h"

#define x (double)(getSpeed() - 120)
#define a 0.0001
#define b 0.02
#define c 1.0

uint8_t getSensorSpeedReading(void) {
    return (uint8_t)(a * x * x + b * x + c);
}