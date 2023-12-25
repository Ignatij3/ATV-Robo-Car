#include "ino_libs.h"
#include "../global_constants/global_constants.h"
#include <avr/cpufunc.h>
#include <avr/interrupt.h>

#define DDx(port) (port - 0x01)
#define PINx(port) (port - 0x02)

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER2A 7
#define TIMER2B 8

static uint8_t pinToTimer(volatile uint8_t *PORT, uint8_t pin);

// initPWMTimers initializes timers 0, 1, 2 for phase Correct PWM signal.
void initPWMTimers(void) {
    // set up timers
    TCCR0A |= _BV(WGM00) | _BV(COM0A1) | _BV(COM0B1);
    // TCCR1A |= _BV(WGM00) | _BV(COM1A1) | _BV(COM1B1);
    // TCCR2A |= _BV(WGM00) | _BV(COM2A1) | _BV(COM2B1);

    // enabling interrupts for all timers
    TIMSK0 |= _BV(TOIE0);
    // TIMSK1 |= _BV(TOIE1);
    // TIMSK2 |= _BV(TOIE2);

    // enable PWM and set prescaler to F_CPU/8
    TCCR0B |= _BV(CS01);
    // TCCR1B |= _BV(CS11);
    // TCCR2B |= _BV(CS21);
}

// initADC initializes built-in analog-digital converter and sets needed prescaler;
void initADC(void) {
    PRR &= ~_BV(PRADC);
    // enable ADC
    ADCSRA = _BV(ADEN);
    // set prescaler = 128
    ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

// pinMode configures pin at specified port to one of three possible configurations.
// PORT parameter must be a pointer to the according port register defined in avr/io.h.
// PORT must represent port at which desired pin exists.
// It is advised to use only INPUT, OUTPUT, INPUT_PULLUP as 'mode' parameter, to prevent bugs.
void pinMode(volatile uint8_t *PORT, uint8_t pin, uint8_t mode) {
    // mode&1 used in case pullup mode passed, to be left with only first bit
    *DDx(PORT) &= ~_BV(pin);         // clear bit
    *DDx(PORT) |= (mode & 1) << pin; // write either 1 or 0

    // setting pullup setting to PORT address/*
    *PORT &= ~_BV(pin);          // clear bit
    *PORT |= (mode >> 1) << pin; // write either 1 or 0 (pullup bit)

    _NOP();
}

// digitalWrite writes either HIGH or LOW to pin at specified PORT.
// PORT parameter must be a pointer to the according port register defined in avr/io.h.
// PORT must represent port at which desired pin exists.
// It is advised to use only HIGH and LOW as 'value' parameter, otherwise function may show unexpected behaviour.
void digitalWrite(volatile uint8_t *PORT, uint8_t pin, uint8_t value) {
    uint8_t mask = *DDx(PORT) & _BV(pin);
    // if the pin is input, don't clear bit
    *PORT &= ~_BV(pin) | ~mask;
    // if pin is input, write nothing, else 1 or 0
    *PORT |= (value << pin) & mask;
}

// digitalRead reads either 1 or 0 from pin at specified PORT.
// PORT parameter must be a pointer to the according port register defined in avr/io.h.
// PORT must represent port at which desired pin exists.
uint8_t digitalRead(volatile uint8_t *PORT, uint8_t pin) {
    return (*PINx(PORT) >> pin) & 1;
}

// analogWrite writes PWM waveform generator duty cycle length (value) to pin connected to PWM at specified PORT.
// PORT parameter must be a pointer to the according port register defined in avr/io.h.
// PORT must represent port at which desired pin exists.
// If passed pin is not PWM pin, function writes value/255 rounded to the nearest integer.
// If pin is not on timer, function writes either 1 or 0,
// depending on which is closer representation.
void analogWrite(volatile uint8_t *PORT, uint8_t pin, uint8_t value) {
    switch (pinToTimer(PORT, pin)) {
    case TIMER0A:
        // set pwm duty cycle
        OCR0A = value;
        break;

    case TIMER0B:
        OCR0B = value;
        break;

    case TIMER1A:
        OCR1A = value;
        break;

    case TIMER1B:
        OCR1B = value;
        break;

    case TIMER2A:
        OCR2A = value;
        break;

    case TIMER2B:
        OCR2B = value;
        break;

    case NOT_ON_TIMER:
        digitalWrite(PORT, pin, HIGH & (value >> 7)); // 0 if <128

    default:
        break;
    }
}

// analogRead reads analog signal from pin at specified PORT.
// PORT parameter must be a pointer to the according port register defined in avr/io.h.
// PORT must represent port at which desired pin exists.
// If pin is not on PWM timer and is not analog pin, function returns 0.
uint16_t analogRead(volatile uint8_t *PORT, uint8_t pin) {
    uint16_t value = 0;
    switch (pinToTimer(PORT, pin)) {
    case TIMER0A:
        // get pwm duty cycle
        value = OCR0A;
        break;

    case TIMER0B:
        value = OCR0B;
        break;

    case TIMER1A:
        value = OCR1A;
        break;

    case TIMER1B:
        value = OCR1B;
        break;

    case TIMER2A:
        value = OCR2A;
        break;

    case TIMER2B:
        value = OCR2B;
        break;

    default:
        if (PORT == &PORTC) {
            // set the analog reference (high two bits of ADMUX) and select the channel (low 4 bits).
            // This also sets ADLAR (left-adjust result) to 0 (the default).
            ADMUX = _BV(REFS0) | (pin & 0b00000111);

            // Start the conversion
            ADCSRA |= _BV(ADSC);

            // ADSC is cleared when the conversion finishes
            while (ADCSRA & _BV(ADSC))
                ;

            // reading result after conversion
            value = ADCL | (ADCH << 8);
        }
    }

    return value;
}

// pulseIn waits for state change of pin at specified PORT.
// pulseIn should be used to monitor time it takes for sent pulse to come back,
// which will be singalled by pin changing state.
// PORT parameter must be a pointer to the according port register defined in avr/io.h.
// PORT must represent port at which desired pin exists.
// If function times out, it returns 0. Otherwise, it returns time it took pulse to return in microseconds.
// It is advised to use only HIGH and LOW as 'value' parameter, otherwise function may show unexpected behaviour.
uint32_t pulseIn(volatile uint8_t *PORT, uint8_t pin, uint8_t state, uint32_t timeout, void (*sendPulse)(void)) {
    // cache the port and bit of the pin in order to speed up the
    // pulse width measuring loop and achieve finer resolution.  calling
    // digitalRead() instead yields much coarser resolution.
    uint8_t bit = _BV(pin);
    uint8_t stateMask = bit & (state << pin);
    uint32_t width = 0; // keep initialization out of time critical area

    // convert the timeout from microseconds to a number of times through
    // the initial loop; it takes 16 clock cycles per iteration.
    uint32_t maxloops = microsecondsToClockCycles(timeout) / 16;

    // wait for any previous pulse to end
    while ((*PINx(PORT) & bit) == stateMask) {
        if (maxloops-- == 0) {
            return 0;
        }
    }

    // start pulse to measure
    sendPulse();

    // wait for the pulse to start
    while ((*PINx(PORT) & bit) != stateMask) {
        if (maxloops-- == 0) {
            return 0;
        }
    }

    // wait for the pulse to end
    while ((*PINx(PORT) & bit) == stateMask) {
        if (maxloops-- == 0) {
            return 0;
        }
        width++;
    }

    // convert the reading to microseconds. The loop has been determined
    // to be 20 clock cycles long and have about 16 clocks between the edge
    // and the start of the loop. There will be some error introduced by
    // the interrupt handlers.
    return clockCyclesToMicroseconds(width * 21 + 16);
}

// pinToTimer matches pin at specific port with PWM timer.
// PORT parameter must be a pointer to the according port register defined in avr/io.h.
// PORT must represent port at which desired pin exists.
// If no timer exists for the pin, the function returns NOT_ON_TIMER.
static uint8_t pinToTimer(volatile uint8_t *PORT, uint8_t pin) {
    if (PORT == &PORTB) {
        if (pin == PINB1) {
            return TIMER1A;
        } else if (pin == PINB2) {
            return TIMER1B;
        } else if (pin == PINB3) {
            return TIMER2A;
        }
    } else if (PORT == &PORTD) {
        if (pin == PIND3) {
            return TIMER2B;
        } else if (pin == PIND5) {
            return TIMER0B;
        } else if (pin == PIND6) {
            return TIMER0A;
        }
    }
    return NOT_ON_TIMER;
}