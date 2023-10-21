#include "../oled/ssd1306.h"
#include "../ino_libs/ino_libs.h"
#include <util/delay.h>
#include <stdio.h>

#define VRX PINC1
#define VRY PINC0
#define SW PIND2

void registerJoystick(void) {
    pinMode(&PORTD, SW, INPUT);
    pinMode(&PORTC, VRX, INPUT);
    pinMode(&PORTC, VRY, INPUT);
}

void lol(){
    char str[20]; 
    while(1){
        int xValue = analogRead(&PORTC, VRX);
        int yValue = analogRead(&PORTC, VRY);
        sprintf(str, "%d", xValue);
        setSpeed_OLED(str);
        delay_ms(1000);
    }
 }