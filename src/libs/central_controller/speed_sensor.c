#include "../speed_sensor/speed_sensor.h"
#include "central_controller.h"

// update the vehicle speed
uint8_t getCarSpeedSensor(void) {
    return getSensorSpeedReading();
}