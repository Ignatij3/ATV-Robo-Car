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

//updating the vehicle speed depending on the time taken to overcome the obstacle
//In case if this time is more than SECOND we assume that the car is not moving
void updateSpeed(void) {
    // Configure external interrupt 0 (INT0) 
    EICRA |= _BV(ISC01);
    EIMSK |= _BV(INT0);
    PCICR |= _BV(PCIE2);   // Enable interrupt for port D 
    PCMSK2 |= _BV(PCINT19); // Enable pin PD3 (PCINT19)
    sei(); // Enable global interrupts
}

// Interrupt service routine for INT0 
ISR(INT0_vect) {
    static uint32_t startTime;
    static uint8_t firstTrigger = 1;

    // if sensor see the obstacle
    if (firstTrigger) {
        startTime = micros();
        firstTrigger = 0;
    } else {
        // 1 mm = 1*10^-3m
        // 1 microsecond = 1*10^-6 s
        // 1 mm/microsecond = 1*10^-3 / 1*10^-6 = 10^3 m/s
        // 1 mm/microsecond = 1000 m/s

        //elapseTime = micros()-startTime;
        uint8_t result = (uint8_t)((LENGTH*(uint32_t)1000)/(micros()-startTime));

        setSpeed_OLED(result); //update the screen with new speed

        firstTrigger = 1; // Reset for the next measurement
    }
}
