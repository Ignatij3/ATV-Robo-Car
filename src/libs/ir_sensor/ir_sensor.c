#include "../engine_controller/engine_controller.h"
#include "../ino_libs/ino_libs.h"
#include "../serial_communication/serial_communication.h"
#include "ir_sensor.h"
#include <avr/io.h>

#define IR_RIGHT PINC1
#define IR_LEFT PINC2

static bool lineOnLeft;
static bool lineOnRight;

// initializeIR initialises IR sensors.
void initializeIR(void) {
    pinMode(&PORTC, IR_LEFT, INPUT);
    pinMode(&PORTC, IR_RIGHT, INPUT);
}

// updateIRReadings reads data from IR sensors, which report whether car is on non-reflective surface.
void updateIRReadings(void) {
    lineOnLeft = !digitalRead(&PORTC, IR_LEFT);
    lineOnRight = !digitalRead(&PORTC, IR_RIGHT);
}

// isOnLine returns whether car is on line.
// Car is thought to be on line if both sensors report that they see a black line.
bool isOnLine(void) {
    return lineOnLeft && lineOnRight;
}

// isOffLine reports if none of the sensors see a black line.
bool isOffLine(void) {
    return !(lineOnLeft || lineOnRight);
}

// isOnLeftOfLine returns true if only right sensor sees a black line.
bool isOnLeftOfLine(void) {
    return !lineOnLeft && lineOnRight;
}

// isOnRightOfLine returns true if only left sensor sees a black line.
bool isOnRightOfLine(void) {
    return lineOnLeft && !lineOnRight;
}