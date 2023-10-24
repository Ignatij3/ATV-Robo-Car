#include "../oled/ssd1306.h"
#include "../ino_libs/ino_libs.h"
#include "../serial_communication/serial_communication.h"
#include <util/delay.h>
#include <stdio.h>

#define VRX PINC1
#define VRY PINC0
#define SW PIND2

void registerJoystick(void) {
    pinMode(&PORTD, SW, INPUT_PULLUP);
    pinMode(&PORTC, VRX, INPUT_PULLUP);
    pinMode(&PORTC, VRY, INPUT_PULLUP);
}

void lol(){
    char str[20];
    while(1){
        uint8_t a = digitalRead(&PORTD, SW);
        writeStringF("%d", a);
        _delay_ms(1900);    
    }

    // while(1){
    //     int xValue = analogRead(&PORTC, VRX);
    //     int yValue = analogRead(&PORTC, VRY);
    //     sprintf(str, "%d", xValue);
    //     setSpeed_OLED(str);
    //     _delay_ms(1000);
    // }
 }

//  void changeMode()