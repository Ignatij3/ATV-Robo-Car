// #include "libs/central_controller/central_controller.h"
// #include "libs/serial_communication/serial_communication.h"
// #include "libs/joystick/joystick.h"
// #include <avr/interrupt.h>
// #include <util/delay.h>

// volatile static bool poweredOn = false;

// // captures interrupt from switch pin, switches power to the wheels on or off.
// ISR(PCINT0_vect) {
//     // interrupt routine is triggered both on rising and falling edge, to distinguish between those,
//     // state variable is introduced, which is toggled every routine execution.
//     // buttons state: 1 - released, 0 - pressed
//     volatile static uint8_t state = 1;

//     // toggle power of the car only if button is pressed
//     state = !state;
//     if (!state) {
//         poweredOn = !poweredOn;
//     }
// }

// // setUpInterrupts sets to registers that manage interrupts to capture interrupts from built-in switch.
// // Afterwards, it enables interrupts.
// void setUpInterrupts(void) {
//     EICRA |= _BV(ISC01);
//     EIMSK |= _BV(INT0);
//     PCICR |= _BV(PCIE0);   // port b
//     PCMSK0 |= _BV(PCINT7); // switch pin
//     sei();                 // enable interrupts
// }

// int main(void) {
//     initializeModules(10);
//     setMode(AUTOMATIC);
//     setUpInterrupts();
//     enableCar();

//     while (1) {
//         // if (buttonPressed){

//         // }
//         // halt while car is turned off
//         if (!poweredOn) {
//             disableCar();
//             while (!poweredOn) {
//             }
//             enableCar();
//         }

//         setMode(readNewMode());

//         switch (getMode()) {
//         // in automatic mode, car drives forward until colliding
//         // afterwards, it turns around and continues forward
//         case AUTOMATIC:
//             accelerate(1);
//             // if (isCollisionSoon()) {
//             //     evadeCollision();
//             // }
//             break;

//         // in controlled mode, car receives and executes commands from DualShock PS4 controller
//         case CONTROLLED:
//             /* code */
//             break;

//         // in slave mode, car follows black line. If there is predecessor on a line, the car tailgates it
//         case SLAVE:
//             /* code */
//             break;

//         // if NONE mode is chosen, the car must halt
//         case NONE:
//             goto exit;
//         }
//         // delay not to change state too rapidly
//         _delay_ms(10);
//     }

// exit:
//     disableCar();
//     writeString("\n\rerror occured, exiting");
//     return 1;
// }

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Define the UART settings
#define F_CPU 16000000UL  // Clock frequency in Hz
#define BAUD 9600         // Baud rate
#define UBRR_VAL (F_CPU / (16 * BAUD)) - 1

void UART_Init(uint32_t ubrr) {
    UBRR0H = (ubrr & 0x0F00) >> 8;
    UBRR0L = (ubrr & 0x00FF);

    UCSR0B |= _BV(TXEN0) | _BV(RXEN0) | _BV(TXCIE0) | _BV(RXCIE0);
    sei();
}

volatile static uint8_t uart_tx_busy = 1;
void UART_Transmit(const char c) {
    enableLED();
    while (uart_tx_busy == 0)
        ;
    uart_tx_busy = 0;
    UDR0 = c;
    disableLED();
}

int main(void) {
    // Initialize UART
    UART_Init(UBRR_VAL);

    // Main program loop
    while (1) {
        // Send data to the serial monitor
        UART_Transmit('H');
        UART_Transmit('e');
        UART_Transmit('l');
        UART_Transmit('l');
        UART_Transmit('o');
        UART_Transmit('!');
        UART_Transmit('\n');  // Newline character for formatting

        // Delay for a short time (optional)
        _delay_ms(1000);
    }
    return 0;
}