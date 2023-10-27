#include "../central_controller/central_controller.h"
#include "../ino_libs/ino_libs.h"
#include "../oled/ssd1306.h"
#include "joystick.h"
#include <avr/io.h>
#include <util/delay.h>

#define VRX PINC1
#define VRY PINC0
#define SW PIND7
#define MAXIMUM 1023
#define ZONE 40      // zone in which the joystick coordinates will be readable
#define INTERVAL 100 // milliseconds

struct Coordinates {
    uint8_t X;
    uint8_t Y;
};

#define LOWER 0
#define CENTER 128
#define UPPER 255

// the coordinates in which joystick reside in different parts
#define RIGHT ((struct Coordinates){LOWER, CENTER})
#define UP ((struct Coordinates){CENTER, UPPER})
#define LEFT ((struct Coordinates){UPPER, CENTER})
#define DOWN ((struct Coordinates){CENTER, LOWER})

// check when the button on joystick is pressed
bool pressed(void) {
    return !digitalRead(&PORTD, SW);
}

// register all pins in joystick
void registerJoystick(void) {
    pinMode(&PORTD, SW, INPUT_PULLUP);
    pinMode(&PORTC, VRX, INPUT);
    pinMode(&PORTC, VRY, INPUT);
}

// map value, which provide joystick(0-1023) to unit8_t format(0-255)
uint8_t mapValue(uint16_t myvalue) {
    return (myvalue >> 2);
}

// This function allows the user to change the operating mode by interacting with a joystick.
// It continuously monitors the joystick's position and changes the mode based on user input.
// It returns the mode, which the user has selected
uint8_t changeMode(void) {
    uint8_t mode = modeMenu_OLED();
    uint32_t startTime = millis(), currentTime = 0;
    uint8_t xValue = mapValue(analogRead(VRX)), yValue = mapValue(analogRead(VRY));

    while (!pressed()) {
        currentTime = millis();
        if (currentTime - startTime >= INTERVAL) {
            startTime = currentTime;
            xValue = mapValue(analogRead(VRX)), yValue = mapValue(analogRead(VRY));
            if (UP.X + ZONE > xValue && UP.X - ZONE < xValue && UP.Y + ZONE > yValue && UP.Y - ZONE < yValue) {
                mode = setMode_OLED(mode, 1);
            } else if (DOWN.X + ZONE > xValue && DOWN.X - ZONE < xValue && DOWN.Y + ZONE > yValue &&
                       DOWN.Y - ZONE < yValue) {
                mode = setMode_OLED(mode, -1);
            }
        }
    }
    infoMenu_OLED();
    return mode;
}
