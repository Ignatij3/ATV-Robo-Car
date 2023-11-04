#include "../ino_libs/ino_libs.h"
#include "central_controller.h"
#include <avr/interrupt.h>

static void setupPowerSwitchInterrupts(void);

static bool poweredOn = false;

// initializeModules performs initialization of structures for the vehicle to operate.
// minDistance is minimal distance between car and object in front,
// to be able to evade collision, imperatively calculated.
// The function sets initial vehicle mode to NONE.
void initializeModules(uint8_t minDistance) {
    // initialize microcontroller
    initPWMTimers();
    initADC();
    setupPowerSwitchInterrupts();

    // initialize other modules
    _controllerInitSerial();
    _controllerPairWithBluetooth();
    _controllerInitEngines();
    _controllerInitDistanceSensor(minDistance);
    _controllerInitIRSensor();
    _controllerInitOLED();
    _controllerInitJoystick();
    _controllerInitSpeedSensor();
}

// captures interrupt from switch pin, switches power to the wheels on or off.
ISR(PCINT0_vect) {
    // interrupt routine is triggered both on rising and falling edge, to distinguish between those,
    // state variable is introduced, which is toggled every routine execution.
    // buttons state: 1 - released, 0 - pressed
    static volatile uint8_t state = 1;

    // toggle power of the car only if button is pressed
    state = !state;
    if (!state) {
        poweredOn = !poweredOn;
    }
}

// setupPowerSwitchInterrupts sets to registers that manage interrupts to capture interrupts from built-in switch.
// Afterwards, it enables interrupts.
static void setupPowerSwitchInterrupts(void) {
    EICRA |= _BV(ISC01);
    EIMSK |= _BV(INT0);
    PCICR |= _BV(PCIE0);   // port b
    PCMSK0 |= _BV(PCINT7); // switch pin
    sei();                 // enable interrupts
}

// isPoweredOn returns whether car is powered on.
bool isPoweredOn(void) {
    return poweredOn;
}