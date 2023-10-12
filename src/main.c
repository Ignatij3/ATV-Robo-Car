#include "libs/distance_sensor/HCSR04.h"
#include "libs/ino_libs/ino_libs.h"
#include "libs/serial_communication/serial_communication.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define BAUD 9600
#define FQ_CPU 16000000UL
#define MYUBRR(baud) FQ_CPU / 16 / baud - 1

int main(void) {
    registerDistanceSensor();
    serialInit(MYUBRR(BAUD));

    char str[5];
    while (1) {
        intToStr(measureDistanceCm(), str);
        writeString(" distance:");
        writeString(str);
        writeByte('\n');
        writeByte('\r');

        _delay_ms(1000);
    }
    return 0;
}