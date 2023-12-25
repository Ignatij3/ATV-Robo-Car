#include "../engine_controller/engine_controller.h"
#include "../global_constants/global_constants.h"
#include "../ir_sensor/ir_sensor.h"
#include "central_controller.h"

int8_t deviation;

// _controllerInitIRSensor initializes IR reflectivity sensor.
void _controllerInitIRSensor(void) {
    initializeIR();
}

// updateLinePosition reads IR sensors' readings to get line's position relative to the Car.
// It returns the said position.
linePosition updateLinePosition(void) {
    deviation = updateIRReadings();
    if (deviation == UNKNOWN) {
        return UNKNOWN;
    } else if (deviation > 0) {
        return RIGHT;
    } else if (deviation < 0) {
        return LEFT;
    }
    return CENTERED;
}

// adjustEnginesSpeed adjusts each side's engine speed to turn the Car in direction of the line.
// Resulting engine power difference is adjustmentFactor multiplied by deviation factor.
void adjustEnginesSpeed(uint8_t adjustmentFactor) {
    if (deviation == 0 || deviation == UNKNOWN) {
        return;
    }

    // general variables for better code clearance
    // outer side is the one which will be doing greater turn when centering on the line
    uint8_t outerSideSpeed, innerSideSpeed;
    bool reverse = isReverse();
    void (*setOuterSpeed)(uint8_t, bool);
    void (*setInnerSpeed)(uint8_t, bool);

    if (deviation > 0) {
        outerSideSpeed = getLeftSpeed();
        *(&setOuterSpeed) = setLeftSpeed;
        innerSideSpeed = getRightSpeed();
        *(&setInnerSpeed) = setRightSpeed;

    } else {
        outerSideSpeed = getRightSpeed();
        *(&setOuterSpeed) = setRightSpeed;
        innerSideSpeed = getLeftSpeed();
        *(&setInnerSpeed) = setLeftSpeed;
        deviation = -deviation;
    }

    uint8_t delta = deviation * adjustmentFactor * 0.5;

    // set inner speed to MIN_SPEED if new speed underflows
    if (innerSideSpeed < MIN_SPEED + delta) {
        innerSideSpeed = MIN_SPEED + delta;
    }

    // set outer speed to MAX_SPEED if new speed overflows
    if (outerSideSpeed > MAX_SPEED - delta) {
        outerSideSpeed = MAX_SPEED - delta;
    }

    setInnerSpeed(innerSideSpeed - delta, reverse);
    setOuterSpeed(outerSideSpeed + delta, reverse);
}