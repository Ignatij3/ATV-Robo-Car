#include "libs/central_controller/central_controller.h"
#include "libs/serial_communication/serial_communication.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include "libs/oled/ssd1306.h"
#include <time.h>

#define INTERVAL 1
static volatile bool poweredOn = false;

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

// setUpInterrupts sets to registers that manage interrupts to capture interrupts from built-in switch.
// Afterwards, it enables interrupts.
void setUpInterrupts(void) {
    EICRA |= _BV(ISC01);
    EIMSK |= _BV(INT0);
    PCICR |= _BV(PCIE0);   // port b
    PCMSK0 |= _BV(PCINT7); // switch pin
    sei();                 // enable interrupts
}

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;

unsigned long millis()
{
	unsigned long m;
	uint8_t oldSREG = SREG;

	// disable interrupts while we read timer0_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer0_millis)
	cli();
	m = timer0_millis;
	SREG = oldSREG;

	return m;
}

unsigned long micros() {
	unsigned long m;
	uint8_t oldSREG = SREG, t;
	
	cli();
    t = TCNT0;
	m = timer0_overflow_count;
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
		m++;

	SREG = oldSREG;
	
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

int main(void) {
    uint16_t time1 = millis();
    uint16_t time2 = micros();
    _delay_ms(1023);
    uint16_t time3 = millis();
    uint16_t time4 = micros();
    writeUint(time1);
    writeUint(time2);
    writeUint(time3);
    writeUint(time4);
}




//     initializeModules(10);
//     setUpInterrupts();
//     setMode(readNewMode());
//     enableCar();
//     time_t timeNow = time(NULL), previousTime = 0;  

//     while (1) {
//         timeNow = time(NULL); //get new time
//         //every second the timer and speed information are updated
//         if (timeNow - previousTime >= INTERVAL){ 
//             previousTime = timeNow;
//             updateCarSpeed();
//             updateTimer();
//         }

//         // halt while car is turned off
//         if (!poweredOn) {
//             disableCar();
//             while (!poweredOn) {
//             }
//             enableCar();
//         }

//         //change driving mode when joystick is pressed
//         if (joystickPressed()){
//             setMode(readNewMode());
//         }

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
//             updateLinePosition();
//             adjustEnginesSpeed(30);
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