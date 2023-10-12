#include "libs/distance_sensor/HCSR04.h"
#include "libs/ino_libs/ino_libs.h"
#include "libs/serial_communication/serial_communication.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define BAUD 9600
#define F_CPU 16000000UL
#define MYUBRR(baud) F_CPU / 16 / baud - 1

int main(void) {
    registerDistanceSensor();
    serialInit(MYUBRR(BAUD));
    uint8_t distance = measureDistanceCm();
    writeString(intToStr(distance));
    return 0;
}