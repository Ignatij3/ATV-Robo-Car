#include "../global_constants/global_constants.h"
#include "../ino_libs/ino_libs.h"
#include "HCSR04.h"
#include <avr/io.h>
#include <util/delay.h>

#define TRIGGER_PIN PINB3
#define ECHO_PIN PINB4

#define MIN_DISTANCE 0

// registerDistanceSensor initializes HCSR04 sensor's pins.
void registerDistanceSensor(void) {
    pinMode(&PORTB, TRIGGER_PIN, OUTPUT);
    pinMode(&PORTB, ECHO_PIN, INPUT);
}

// sendPulse function is created to send a trigger signal to an ultrasonic
// distance sensor. This signal initiates the sensor's pulse.
void sendPulse(void) {
    digitalWrite(&PORTB, TRIGGER_PIN, LOW);
    _delay_us(2);
    // Hold trigger for 10 microseconds, which is signal for sensor to measure distance
    digitalWrite(&PORTB, TRIGGER_PIN, HIGH);
    _delay_us(10);
    digitalWrite(&PORTB, TRIGGER_PIN, LOW);
}

// measureDistanceCm return distance in cm from nearest object in front.
uint8_t measureDistanceCm(void) {
    // Measure the length of echo signal, which is equal to the time needed for sound to go there and back.
    // Using timeout since we can't measure beyond max distance.
    // Compute max delay based on max distance with 25% margin in microseconds.
    // 18586.0035551 = 2.5 * MAX_DISTANCE / 0.0343000042 (rounded up to nearest tenth thousand)
    uint16_t durationMicroSec = (uint16_t)(pulseIn(&PORTB, ECHO_PIN, HIGH, 20000, sendPulse));

    uint16_t distanceCm = (uint16_t)((durationMicroSec * 1715) / 100000);

    static uint16_t lastMeasure = 0;
    if (lastMeasure == 0) {
        lastMeasure = distanceCm;
    }

    // evening out the fluctuations in sensors' readings
    if (distanceCm > lastMeasure && distanceCm - lastMeasure >= 10) {
        lastMeasure++;
        distanceCm = lastMeasure;
    } else if (lastMeasure - distanceCm >= 10) {
        lastMeasure--;
        distanceCm = lastMeasure;
    }

    if (distanceCm == MIN_DISTANCE || distanceCm > MAX_DISTANCE) {
        return MIN_DISTANCE;
    }

    return (uint8_t)(distanceCm);
}