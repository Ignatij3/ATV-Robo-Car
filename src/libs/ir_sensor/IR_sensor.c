#include "IR_sensor.h"
#include "../engine_controller/engine_controller.h"
#include "../ino_libs/ino_libs.h"
#include "../serial_communication/serial_communication.h"
#include <avr/io.h>
#define leftIR PINC1
#define rightIR PINC2
#define black 1
#define white 0

// #define centerIR PINC3

// IR sensor declaration
bool IR_L;
bool IR_R;
// bool IR_C;

void initializeIR() {
    pinMode(&PORTC, leftIR, INPUT);
    pinMode(&PORTC, rightIR, INPUT);
    // pinMode(&PORTC, centerIR, INPUT);
}

void read_sensor(bool *IR_L, bool *IR_R) {
    *IR_L = digitalRead(&PORTC, leftIR);
    *IR_R = digitalRead(&PORTC, rightIR);
}
