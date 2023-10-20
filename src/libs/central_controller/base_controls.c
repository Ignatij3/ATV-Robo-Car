#include "../distance_sensor/HCSR04.h"
#include "../engine_controller/engine_controller.h"
#include "../global_constants/global_constants.h"
#include "../ino_libs/ino_libs.h"
#include "../oled/ssd1306.h"
#include "../serial_communication/serial_communication.h"
#include "base_controls.h"

#define MYUBRR(baud) CPU_CLOCK / 16 / baud - 1

static uint8_t minDist;
static drivingMode mode;

// initializeModules performs initialization of structures for the vehicle to operate.
// minDistance is minimal distance between car and object in front,
// to be able to evade collision, imperatively calculated.
// The function sets initial vehicle mode to NONE.
void initializeModules(uint8_t minDistance) {
    minDist = minDistance;
    mode = NONE;

    // initialize other modules
    // SSD1306_Init(OLED_ADDRESS);
    initPWMTimers();
    enablePWM();
    initializeEngines();
    registerDistanceSensor();
    serialInit(MYUBRR(SERIAL_BAUD));
    initializeIR();
}

// readNewMode reads signal from joystick that is it pushed.
// It returns selected driving mode.
drivingMode readNewMode(void) {
    return AUTOMATIC;
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