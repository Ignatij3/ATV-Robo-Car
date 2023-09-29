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

    PCICR |= PCIE0; // port b
    PCMSK0 |= PCINT7;

    while (1) {
        run();
    }
    return 0;
}
