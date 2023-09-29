#include "libs/central_controller/central_controller.h"
#include "libs/ino_libs/ino_libs.h"
#include <avr/interrupt.h>

ISR(PCINT0_vect) {
    volatile static uint8_t state = 1;

    state = (state - 1) & 1;
    if (!state) {
        togglePower();
    }
}

int main(void) {
    initializeModules(10);
    setMode(AUTOMATIC);

    pinMode(&PORTB, PINB7, INPUT_PULLUP);
    pinMode(&PORTB, PINB5, OUTPUT);

    EICRA |= _BV(ISC01);
    EIMSK |= _BV(INT0);
    PCICR |= _BV(PCIE0); // port b
    PCMSK0 |= _BV(PCINT7);
    sei(); // enable interrupts

    while (1) {
        run();
    }
    return 0;
}
