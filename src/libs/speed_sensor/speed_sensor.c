#include "../central_controller/central_controller.h"
#include "../ino_libs/ino_libs.h"
#include "speed_sensor.h"

#define SPEED_SENSOR PIND3
#define SECOND 1000000 // hw many microseconds are in second
#define LENGTH 5       // the length of our obstacle in mm

static void setupSensorInterrupts(void);

static uint8_t actualSpeed;

// initialises speed sensor
void registerSpeedSensor(void) {
    pinMode(&PORTD, SPEED_SENSOR, INPUT_PULLUP);
    setupSensorInterrupts();
}

uint8_t getSensorSpeedReading(void) {
    return actualSpeed;
}

// updating the vehicle speed depending on the time taken to overcome the obstacle
// In case if this time is more than SECOND we assume that the car is not moving
static void setupSensorInterrupts(void) {
    // Configure external interrupt 0 (INT0)
    EICRA |= _BV(ISC01);
    EIMSK |= _BV(INT0);
    PCICR |= _BV(PCIE2);    // Enable interrupt for port D
    PCMSK2 |= _BV(PCINT19); // Enable pin PD3 (PCINT19)
    sei();                  // Enable global interrupts
}

// Interrupt service routine for optical interrupt sensor
ISR(PCINT2_vect) {
    static uint32_t startTime;

    // if receiver gets signal
    if (digitalRead(&PORTD, SPEED_SENSOR)) {
        // 1 mm = 1*10^-3m
        // 1 microsecond = 1*10^-6 s
        // 1 mm/microsecond = 1*10^-3 / 1*10^-6 = 10^3 m/s
        // 1 mm/microsecond = 1000 m/s

        // elapseTime = micros()-startTime;
        actualSpeed = (uint8_t)((LENGTH * (uint32_t)1000) / (micros() - startTime));
    } else {
        startTime = micros();
    }
}
