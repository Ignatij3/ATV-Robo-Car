#include "central_controller.h"
#include "../speed_sensor/speed_sensor.h"

//initializes speed sensor
void _controllerInitSpeedSensor(void){
    registerSpeedSensor();
}

// update the vehicle speed
void updateCarSpeed(void){
    updateSpeed();
}