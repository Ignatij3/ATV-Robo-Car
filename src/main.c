#include "libs/central_controller/central_controller.h"
#include "libs/engine_controller/engine_controller.h"
#include <avr/interrupt.h>

// captures interrupt from switch pin, switches power to the wheels on or off.
ISR(PCINT0_vect) {
    // button's state: 1 - released, 0 - pressed
    volatile static uint8_t state = 1;

    state = (state - 1) & 1;
    // We don't want to toggle power mode on release
    if (!state) {
        togglePower();
    }
}

// setUpInterrupts writes to registers that manage interrupts to capture interrupts from built-in switch.
// Afterwards, it enables interrupts.
void setUpInterrupts(void) {
    EICRA |= _BV(ISC01);
    EIMSK |= _BV(INT0);
    PCICR |= _BV(PCIE0);   // port b
    PCMSK0 |= _BV(PCINT7); // switch pin
    sei();                 // enable interrupts
}

int main(void) {
    initializeModules(10);
    setMode(AUTOMATIC);
    setUpInterrupts();

    while (1) {
        run();
    }
    return 0;
}