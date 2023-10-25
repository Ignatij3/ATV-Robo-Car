#include "central_controller.h"
#include "../speed_sensor/speed_sensor.h"

void _controllerInitSpeedSensor(void){
    registerSpeedSensor();
}

void updateCarSpeed(void){
    updateSpeed();
}