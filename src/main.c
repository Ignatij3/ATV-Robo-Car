#include "libs/central_controller/central_controller.h"
#include "libs/serial_communication/serial_communication.h"
#include <avr/interrupt.h>
#include <util/delay.h>

bool poweredOn = false;

// captures interrupt from switch pin, switches power to the wheels on or off.
ISR(PCINT0_vect) {
    // button's state: 1 - released, 0 - pressed
    volatile static uint8_t state = 1;

    state = (state - 1) & 1;
    // We don't want to toggle power mode on release
    if (!state) {
        poweredOn = !poweredOn;
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
    enableCar();

    while (1) {
        // halt while car is turned off
        while (!poweredOn) {
        }

        setMode(readNewMode());

        switch (getMode()) {
        // in automatic mode, car drives forward until colliding
        // afterwards, it turns around and continues forward
        case AUTOMATIC:
            accelerate(1);
            if (isCollisionSoon()) {
                evadeCollision();
            }
            break;

        // in controlled mode, car receives and executes commands from DualShock PS4 controller
        case CONTROLLED:
            /* code */
            break;

        // in slave mode, car follows black line. If there is predecessor on a line, the car tailgates it
        case SLAVE:
            /* code */
            break;

        // if NONE mode is chosen, the car must halt
        case NONE:
            writeString("mode is none\n\r");
            return 1;
        }
        // delay not to change state too rapidly
        _delay_ms(10);
    }

    disableCar();
    return 0;
}