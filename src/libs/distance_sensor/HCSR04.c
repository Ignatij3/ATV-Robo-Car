#include "../ino_libs/ino_libs.h"
#include "../global_constants/global_constants.h"
#include "../ino_libs/ino_libs.h"
#include "../serial_communication/serial_communication.h"
#include "HCSR04.h"
#include <avr/io.h>
#include <util/delay.h>

#define TRIGGER_PIN PINB2
#define ECHO_PIN PINB1

// registerDistanceSensor initializes HCSR04 sensor's pins.
void registerDistanceSensor(void) {
    pinMode(&PORTB, TRIGGER_PIN, OUTPUT);
    pinMode(&PORTB, ECHO_PIN, INPUT);
}

void timeTravel() {
    digitalWrite(&PORTB, TRIGGER_PIN, LOW);
    _delay_us(2);
    // Hold trigger for 10 microseconds, which is signal for sensor to measure distance
    digitalWrite(&PORTB, TRIGGER_PIN, HIGH);
    _delay_us(10);
    digitalWrite(&PORTB, TRIGGER_PIN, LOW);
}

// measureDistanceCm return distance in cm from nearest object in front.
uint8_t measureDistanceCm(void) {
    // Make sure that trigger pin is LOW.
    digitalWrite(&PORTB, TRIGGER_PIN, LOW);
    _delay_us(2);
    // Hold trigger for 10 microseconds, which is signal for sensor to measure distance
    digitalWrite(&PORTB, TRIGGER_PIN, HIGH);
    _delay_us(10);
    digitalWrite(&PORTB, TRIGGER_PIN, LOW);

    // Compute max delay based on max distance with 25% margin in microseconds
    // 18586.0035551 = 2.5 * MAX_DISTANCE / 0.0343000042

    // Measure the length of echo signal, which is equal to the time needed for
    // sound to go there and back.
    uint32_t durationMicroSec = pulseIn(&PORTB, ECHO_PIN, HIGH, 18586, timeTravel); // can't measure beyond max distance

    char portStr[10]; // uint32_t distanceCm = durationMicroSec;
    intToStr(durationMicroSec, portStr);
    writeString(" duration:  ");
    writeString(portStr);
    uint8_t distanceCm = (uint8_t)((durationMicroSec * (uint32_t)(1715)) / (uint32_t)(100000));

    if (distanceCm == 0 || distanceCm > 255) {
        return UINT8_MAX;
    }
    return (uint8_t)(distanceCm);
}