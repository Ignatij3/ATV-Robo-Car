#include "libs/central_controller/central_controller.h"
#include "libs/ino_libs/ino_libs.h"
#include <avr/interrupt.h>
#include <avr/io.h>

ISR(PCINT0_vect) {
    togglePower();
}

int main(void) {
    initializeModules(10);
    sei(); // enable interrupts
    setMode(AUTOMATIC);

    pinMode(&PORTB, PINB7, INPUT_PULLUP);

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
