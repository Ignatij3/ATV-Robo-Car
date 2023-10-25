#include "../ino_libs/ino_libs.h"
#include "../oled/ssd1306.h"
#include "speed_sensor.h"


#define SPEED_SENSOR PIND3

void registerSpeedSensor(void){
    pinMode(&PORTD, SPEED_SENSOR, INPUT_PULLUP);
}

// volatile uint32_t milliseconds = 0;

// // Function to initialize the timer and enable interrupts
// void initTimer() {
//     // Set up timer 0
//     TCCR0B = (1 << CS01);

//     // Enable overflow interrupt
//     TIMSK0 |= (1 << TOIE0);

//     // Activate interrupts
//     sei();
// }

// // Function to get the current time in milliseconds
// uint32_t millis() {
//     uint32_t ms;
//     cli();
//     ms = milliseconds;
//     sei();
//     return ms;
// }

// ISR(TIMER0_OVF_vect) {
//     // This gets called every time there is an overflow in the timer register
//     milliseconds += 1; // Increase milliseconds by 1
// }

// void updateSpeed(void){
    
//     while (!digitalRead(&PORTD, SPEED_SENSOR)){
//     }
//     initTimer();
//     uint32_t currentTime = millis();
//     while (digitalRead(&PORTD, SPEED_SENSOR)){
        
//     }
//     uint32_t elapseTime = millis()-currentTime;
//     //formula 
//     //setSpeed_OLED(formula);
//     return;
// }

// #define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )

// volatile unsigned long timer0_overflow_count = 0;
// volatile unsigned long timer0_millis = 0;

// unsigned long millis()
// {
// 	unsigned long m;
// 	uint8_t oldSREG = SREG;

// 	// disable interrupts while we read timer0_millis or we might get an
// 	// inconsistent value (e.g. in the middle of a write to timer0_millis)
// 	cli();
// 	m = timer0_millis;
// 	SREG = oldSREG;

// 	return m;
// }

// unsigned long micros() {
// 	unsigned long m;
// 	uint8_t oldSREG = SREG, t;
	
// 	cli();
// 	m = timer0_overflow_count;
// 	#error TIMER 0 not defined

// 	if ((TIFR & _BV(TOV0)) && (t < 255))
// 		m++;

// 	SREG = oldSREG;
	
// 	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
// }