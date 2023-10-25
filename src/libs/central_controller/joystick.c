#include "central_controller.h"
#include "../joystick/joystick.h"

static drivingMode mode;

// _controllerInit initializes on-board joystick.
void _controllerInitJoystick(void) {
    registerJoystick();
}

// readNewMode reads signal from joystick that is it pushed.
// It returns selected driving mode.
drivingMode readNewMode(void) {
    return changeMode(); // read from joystick
}

//check when the button on joystick is pressed
bool joystickPressed(void){
    return pressed();
}

// setMode sets passed driving mode.
// If driving mode is NONE, setMode wouldn't accept it.
void setMode(drivingMode newMode) {
    if (newMode != NONE) {
        mode = newMode;
    }
}

// getMode returns driving mode.
drivingMode getMode(void) {
    return mode;
}

