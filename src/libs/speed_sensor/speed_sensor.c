#include "../ino_libs/ino_libs.h"
#include "../oled/ssd1306.h"
#include "../central_controller/central_controller.h"
#include "speed_sensor.h"

#define SPEED_SENSOR PIND3
#define SECOND 1000000 //hw many microseconds are in second
#define LENGTH 5 // the length of our obstacle in mm

//initialises speed sensor
void registerSpeedSensor(void){
    pinMode(&PORTD, SPEED_SENSOR, INPUT_PULLUP);
}

// update cars speed using the time in which the wheel is rotated 1 obstacle
void updateSpeed(void){
    while (!digitalRead(&PORTD, SPEED_SENSOR)){
    }
    uint32_t startTime = micros();
    while (digitalRead(&PORTD, SPEED_SENSOR)){
        if (micros()-startTime>SECOND) {
            setSpeed_OLED(0);
            return;
        }    
    }
    // 1 mm = 1*10^-3m
    // 1 microsecond = 1*10^-6 s
    // 1 mm/microsecond = 1*10^-3 / 1*10^-6 = 10^3 m/s

    // 1 mm/microsecond = 1000 m/s

    //elapseTime = micros()-startTime;

    uint8_t result = (uint8_t)((LENGTH*(uint32_t)1000)/(micros()-startTime));
    setSpeed_OLED(result);
    return;
}
