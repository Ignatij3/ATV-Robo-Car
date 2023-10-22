#include "central_controller.h"

static drivingMode mode;

// _controllerInit initializes on-board joystick.
void _controllerInitJoystick(drivingMode drMode) {
    mode = drMode;
}

// readNewMode reads signal from joystick that is it pushed.
// It returns selected driving mode.
drivingMode readNewMode(void) {
    return AUTOMATIC; // TODO: read from joystick
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