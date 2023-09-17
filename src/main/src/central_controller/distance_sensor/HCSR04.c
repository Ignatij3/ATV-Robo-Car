#include "Arduino.h"
#include "HCSR04.h"

typedef struct {
    uint8_t triggerPin;
    uint8_t echoPin;
    uint16_t maxTimeoutMicroSec;
} distanceSensor;

static distanceSensor sensor;

// registerDistanceSensor initializes HCSR04 sensor.
void registerDistanceSensor(uint8_t triggerPin, uint8_t echoPin,
                            uint16_t maxTimeoutMicroSec) {
    sensor.triggerPin = triggerPin;
    sensor.echoPin = echoPin;
    sensor.maxTimeoutMicroSec = maxTimeoutMicroSec;
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

// measureDistanceCm return distance in cm from nearest object in front.
uint8_t measureDistanceCm(void) {
    // Make sure that trigger pin is LOW.
    digitalWrite(sensor.triggerPin, LOW);
    delayMicroseconds(2);
    // Hold trigger for 10 microseconds, which is signal for sensor to measure
    // distance.
    digitalWrite(sensor.triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(sensor.triggerPin, LOW);

    // Compute max delay based on max distance with 25% margin in microseconds
    // 18586.0035551 = 2.5 * 255 / 0.0343000042

    // Measure the length of echo signal, which is equal to the time needed for
    // sound to go there and back.
    uint32_t durationMicroSec =
        pulseIn(sensor.echoPin, HIGH,
                18586); // can't measure beyond max distance

    uint16_t distanceCm =
        (uint16_t)((uint32_t)(durationMicroSec * (uint32_t)(1715)) /
                   (uint32_t)(100000));
    if (distanceCm == 0 || distanceCm > 255) {
        return UINT8_MAX;
    }

    return (uint8_t)(distanceCm);
}