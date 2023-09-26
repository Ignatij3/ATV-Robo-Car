#include "libs/central_controller/central_controller.h"
#include <avr/interrupt.h>
#include <util/delay.h>

int main() {
    initializeModules(10);
    sei(); // enable interrupts

    while (1) {
        setMode(AUTOMATIC);
        run();
    }
    return 0;
}