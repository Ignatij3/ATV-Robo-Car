#include "../engine_controller/engine_controller.h"
#include "../joystick/joystick.h"
#include "central_controller.h"

static drivingMode mode;

// _controllerInit initializes on-board joystick.
void _controllerInitJoystick(void) {
    registerJoystick();
}

// updateMode stops car, reads new mode from joystick and resumes driving.
// updateMode reads signal from joystick that is it pushed.
// It returns selected driving mode.
drivingMode updateMode(void) {
    uint8_t leftSpeed = getLeftSpeed();
    uint8_t rightSpeed = getRightSpeed();
    setSpeed(0, isReverse());

    mode = changeMode();

    setLeftSpeed(leftSpeed, isReverse());
    setRightSpeed(rightSpeed, isReverse());
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
