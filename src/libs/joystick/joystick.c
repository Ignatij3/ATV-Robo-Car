#include "../ino_libs/ino_libs.h"
#include "../oled/ssd1306.h"
#include "../serial_communication/serial_communication.h"
#include <stdio.h>
#include <util/delay.h>

#define VRX PINC1
#define VRY PINC0
#define SW PIND2

void registerJoystick(void) {
    pinMode(&PORTD, SW, INPUT);
    pinMode(&PORTC, VRX, INPUT);
    pinMode(&PORTC, VRY, INPUT);
}

void lol() {
    char str[20];
    while (1) {
        int xValue = analogRead(&PORTC, VRX);
        int yValue = analogRead(&PORTC, VRY);
        writeStringF("x: %d | y: %d\n\r", xValue, yValue);
        _delay_ms(1000);
    }
}