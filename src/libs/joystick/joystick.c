#include "../ino_libs/ino_libs.h"
#include "../oled/ssd1306.h"
#include "joystick.h"

#define VRX PINC1
#define VRY PINC0
#define SW PIND2
#define MAXIMUM 1023
#define UINT8T_MAX 255
#define ZONE 10
#define INTERVAL 300

struct Coordinates {
    uint8_t X;
    uint8_t Y;
};

#define RIGHT ((struct Coordinates){0, 128})
#define UP ((struct Coordinates){128, 255})
#define DOWN ((struct Coordinates){128, 0})

bool pressed(void){
    return digitalRead(&PORTD, SW);
}

void registerJoystick(void) {
    pinMode(&PORTD, SW, INPUT_PULLUP);
    pinMode(&PORTC, VRX, INPUT_PULLUP);
    pinMode(&PORTC, VRY, INPUT_PULLUP);
}

uint8_t mapValue(uint16_t myvalue){
    return myvalue / MAXIMUM * UINT8T_MAX;
}

uint8_t changeMode(){
    uint8_t mode = modeMenu_OLED();
    time_t startTime, currentTime = (time(NULL), 0); 
    uint8_t xValue = mapValue(analogRead(&PORTC, VRX)), yValue = mapValue(analogRead(&PORTC, VRY));
    while(RIGHT.X + ZONE < xValue && RIGHT.Y + ZONE < yValue && RIGHT.Y - ZONE > yValue){
        currentTime = time(NULL);
        if (currentTime - startTime >= INTERVAL) {
            startTime = currentTime;
            xValue = mapValue(analogRead(&PORTC, VRX)), yValue = mapValue(analogRead(&PORTC, VRY));
            if (UP.X + ZONE > xValue && UP.X - ZONE < xValue && UP.Y + ZONE > yValue && UP.Y - ZONE < yValue){
                mode = setMode_OLED(mode, -1);
            } else if (DOWN.X + ZONE > xValue && DOWN.X - ZONE < xValue && DOWN.Y + ZONE > yValue && DOWN.Y - ZONE < yValue){
                mode = setMode_OLED(mode, 1);
            }
        }
    }
    infoMenu_OLED();
    return mode;
}
