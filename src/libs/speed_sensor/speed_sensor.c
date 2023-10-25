#include "../ino_libs/ino_libs.h"
#include "../oled/ssd1306.h"
#include "speed_sensor.h"


#define SPEED_SENSOR PIND3

void registerSpeedSensor(void){
    pinMode(&PORTD, SPEED_SENSOR, INPUT_PULLUP);
}

volatile uint32_t milliseconds = 0;

// Function to initialize the timer and enable interrupts
void initTimer() {
    // Set up timer 0
    TCCR0B = (1 << CS01);

    // Enable overflow interrupt
    TIMSK0 |= (1 << TOIE0);

    // Activate interrupts
    sei();
}

// Function to get the current time in milliseconds
uint32_t millis() {
    uint32_t ms;
    cli();
    ms = milliseconds;
    sei();
    return ms;
}

ISR(TIMER0_OVF_vect) {
    // This gets called every time there is an overflow in the timer register
    milliseconds += 1; // Increase milliseconds by 1
}

void updateSpeed(void){
    
    while (!digitalRead(&PORTD, SPEED_SENSOR)){
    }
    initTimer();
    uint32_t currentTime = millis();
    while (digitalRead(&PORTD, SPEED_SENSOR)){
        
    }
    uint32_t elapseTime = millis()-currentTime;
    //formula 
    //setSpeed_OLED(formula);
    return;
}