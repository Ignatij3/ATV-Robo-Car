#include "../engine_controller/engine_controller.h"
#include "../joystick/joystick.h"
#include "central_controller.h"

static drivingMode mode = AUTOMATIC;
static bool choosingMode;

// _controllerInit initializes on-board joystick.
void _controllerInitJoystick(void) {
    registerJoystick();
}

// updateMode stops car, reads new mode from joystick and resumes driving.
// updateMode reads signal from joystick that is it pushed.
// It returns selected driving mode.
drivingMode updateMode(void) {
    choosingMode = true;
    uint8_t leftSpeed = getLeftSpeed();
    uint8_t rightSpeed = getRightSpeed();
    setSpeed(0, isReverse());

    // waiting for user to let go of joystick
    while (joystickPressed()) {
    }
    mode = changeMode();
    while (joystickPressed()) {
    }

    setLeftSpeed(leftSpeed, isReverse());
    setRightSpeed(rightSpeed, isReverse());
    choosingMode = false;
    return mode;
}

// check when the button on joystick is pressed
bool joystickPressed(void) {
    return pressed();
}

// getMode returns driving mode.
drivingMode getMode(void) {
    return mode;
}

// isChoosingMode returns whether user is in process of choosing mode.
bool isChoosingMode(void) {
    return choosingMode;
}