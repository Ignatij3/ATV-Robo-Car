#include "../ino_libs/ino_libs.h"
#include "../serial_communication/serial_communication.h"
#include "ir_sensor.h"
#include <avr/io.h>

#define SENSOR_AMOUNT 2
#define LEFTMOST_IR 0
#define RIGHTMOST_IR SENSOR_AMOUNT - 1

#define IR_LEFT_FIRST PINC2
#define IR_RIGHT_FIRST PINC1

// sensorLookup is an array containing information about physical sensor location.
// Sensors are arranged so leftmost will be first and rightmost will be last.
static const uint8_t sensorLookup[SENSOR_AMOUNT] = {IR_LEFT_FIRST, IR_RIGHT_FIRST};

static uint8_t calculateDeviation(uint8_t sensorArray);
static uint8_t findClosestFiredSensor(uint8_t sensorArray, uint8_t lastSensorArr);

// initializeIR initialises IR sensors.
void initializeIR(void) {
    for (uint8_t sensor = 0; sensor < SENSOR_AMOUNT; sensor++) {
        pinMode(&PORTC, sensorLookup[sensor], INPUT);
    }
}

// updateIRReadings reads data from IR sensors, which report whether car is on non-reflective surface.
// It returns deviation - sensor's position relative to the central sensor.
// If sensor amount is even, deviation is 0 only if both central sensors saw the line.
// Negative value means line was seen on the left of the Car, positive value - otherwise.
int8_t updateIRReadings(void) {
    // sensorArray is a bit mask, where each bit represents one sensor.
    // Leftmost sensor is in the least significant bit.
    static uint8_t sensorArray;
    static int8_t deviation;

    // save and clear previous value
    uint8_t lastSensorArr = sensorArray;
    sensorArray = 0;

    for (uint8_t sensor = 0; sensor < SENSOR_AMOUNT; sensor++) {
        sensorArray |= !digitalRead(&PORTC, sensorLookup[sensor]) << sensor;
    }

    // exactly one sensor saw the line
    if (!(sensorArray & (sensorArray - 1))) {
        deviation = calculateDeviation(sensorArray);
        return deviation;
    }

    // none of the sensors saw the line
    if (sensorArray == 0) {
        if (lastSensorArr && _BV(LEFTMOST_IR)) {
            deviation = LINE_TOO_FAR_LEFT;

        } else if (lastSensorArr && _BV(RIGHTMOST_IR)) {
            deviation = LINE_TOO_FAR_RIGHT;

        } else {
            // trying to find out on which side sensor has last seen line
            sensorArray = findClosestFiredSensor(_BV(1) | _BV(SENSOR_AMOUNT), lastSensorArr);
            // assuming deviation is equal to the found sensor
            deviation = calculateDeviation(sensorArray);
        }
    }

    // more than one sensor saw the line
    if (sensorArray & (sensorArray - 1)) {
        // both sensors are in center
        if (SENSOR_AMOUNT % 2 == 0 &&
            (sensorArray & _BV(SENSOR_AMOUNT / 2 - 1) && sensorArray & _BV(SENSOR_AMOUNT / 2 + 1))) {
            deviation = LINE_CENTERED;
            sensorArray = _BV(SENSOR_AMOUNT / 2 - 1);

        } else {
            sensorArray = findClosestFiredSensor(sensorArray, lastSensorArr);
            deviation = calculateDeviation(sensorArray);
        }
    }

    return deviation;
}

// maxDeviation returns maximal deviation under normal conditions.
// This value represents rightmost sensor seeing the black line.
// If the car wandered left off the line and it is not found by sensors,
// the deviation returned is equal to LINE_TOO_FAR_RIGHT.
int8_t maxDeviation(void) {
    return SENSOR_AMOUNT / 2;
}

// maxDeviation returns minimal deviation under normal conditions.
// This value represents leftmost sensor seeing the black line, and is always negative.
// If the car wandered right off the line and it is not found by sensors,
// the deviation returned is equal to LINE_TOO_FAR_LEFT.
int8_t minDeviation(void) {
    return -(SENSOR_AMOUNT / 2);
}

// calculateDeviation returns sensor's deviation from sensorArray mask.
static uint8_t calculateDeviation(uint8_t sensorArray) {
    // we determine actual deviation by sensor position in the array - the closer sensor to the right,
    // the bigger (by value) is deviation.
    uint8_t devn = minDeviation();
    while (sensorArray != 1) {
        devn++;
        sensorArray >>= 1;
    }
    return devn;
}

// findClosestFiredSensor returns bit mask with closest sensor (which saw the line)
// to the sensor which saw the line on the previous update.
static uint8_t findClosestFiredSensor(uint8_t sensorArray, uint8_t lastSensorArr) {
    uint8_t sensorMask = 0;
    uint8_t smallestDistanceIndex = 0;
    uint8_t smallestDistanceBits = UINT8_MAX;

    for (uint8_t sensorIndex = 0; sensorIndex <= SENSOR_AMOUNT; sensorIndex++) {
        sensorMask = sensorArray & _BV(sensorIndex);

        if (sensorMask) {
            if (lastSensorArr > sensorMask && lastSensorArr / sensorMask < smallestDistanceBits) {
                smallestDistanceBits = lastSensorArr / sensorMask;
                smallestDistanceIndex = sensorIndex;

            } else if (lastSensorArr < sensorMask && sensorMask / lastSensorArr < smallestDistanceBits) {
                smallestDistanceBits = sensorMask / lastSensorArr;
                smallestDistanceIndex = sensorIndex;

            } else if (lastSensorArr == sensorMask) {
                smallestDistanceIndex = sensorIndex;
                break;
            }
        }
    }

    return _BV(smallestDistanceIndex);
}