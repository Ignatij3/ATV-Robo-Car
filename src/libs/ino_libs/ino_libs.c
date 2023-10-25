#include "../global_constants/global_constants.h"
#include "../oled/ssd1306.h"
#include "../serial_communication/serial_communication.h"
#include "ino_libs.h"
#include <avr/cpufunc.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#define DDx(port) (port - 0x01)
#define PINx(port) (port - 0x02)

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define clockCyclesPerMicrosecond() (CPU_CLOCK / 1000000L)
#define clockCyclesToMicroseconds(a) ((a) / clockCyclesPerMicrosecond())
#define microsecondsToClockCycles(a) ((a)*clockCyclesPerMicrosecond())

// the prescaler is set so that timer0 ticks every 8 clock cycles, and the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(8 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile uint32_t timer0_overflow_count = 0;
volatile uint32_t timer0_millis = 0;
static uint8_t timer0_fract = 0;

ISR(TIMER0_OVF_vect) {
    // copy these to local variables so they can be stored in registers
    // (volatile variables must be read from memory on every access)
    uint32_t m = timer0_millis;
    uint8_t f = timer0_fract;

    m += MILLIS_INC;
    f += FRACT_INC;
    if (f >= FRACT_MAX) {
        f -= FRACT_MAX;
        m++;
    }

    timer0_fract = f;
    timer0_millis = m;
    timer0_overflow_count++;
}

#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER2A 7
#define TIMER2B 8

static uint8_t pinToTimer(volatile uint8_t *PORT, uint8_t pin);
static void writeToTimer(volatile uint8_t *PORT, uint8_t pin, uint8_t value);

// initPWMTimers initializes timers 0, 1, 2 for phase Correct PWM signal.
void initPWMTimers(void) {
    // set up timers
    TCCR0A |= _BV(WGM00) | _BV(COM0A1);
    TCCR1A |= _BV(WGM00) | _BV(COM1A1);
    TCCR2A |= _BV(WGM00) | _BV(COM2A1);

    // enabling interrupts for all timers
    TIMSK0 |= _BV(TOIE0);
    // TIMSK1 |= _BV(TOIE1);
    // TIMSK2 |= _BV(TOIE2);

    // enable PWM and prescaler to F_CPU/8
    TCCR0B |= _BV(CS01);
    TCCR1B |= _BV(CS11);
    TCCR2B |= _BV(CS21);
}

// initADC initializes built-in analog-digital converter and sets needed prescaler;
void initADC(void) {
    PRR &= ~_BV(PRADC);
    // enable ADC
    ADCSRA = _BV(ADEN);
    // set prescaler = 128
    ADCSRA |= _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
}

// millis returns milliseconds elapsed from program's startup.
uint32_t millis(void) {
    uint32_t m;

    // disable interrupts while we read timer0_millis or we might get an
    // inconsistent value (e.g. in the middle of a write to timer0_millis)
    cli();
    m = timer0_millis;
    sei();

    return m;
}

// millis returns microseconds elapsed from program's startup.
uint32_t micros(void) {
    uint32_t m;
    uint8_t t;

    cli();
    t = TCNT0;
    m = timer0_overflow_count;
    if ((TIFR0 & _BV(TOV0)) && (t < 255)) {
        m++;
    }
    sei();

    // (m << 8) + t ==> timer increment amount
    // 8 / cyclePerMicroS ==> microseconds per overflow
    uint32_t total = (((m << 8) + t) * 8) / clockCyclesPerMicrosecond();
    return total;
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
void analogWrite(volatile uint8_t *PORT, uint8_t pin, uint8_t value) {
    if (value == 0) {
        digitalWrite(PORT, pin, LOW);
    } else if (value == 255) {
        digitalWrite(PORT, pin, HIGH);
    } else {
        writeToTimer(PORT, pin, value);
    }
}

// writeToTimer sets PWM duty cycle for specific timer provided pin is using.
// If pin is not on timer, function writes either 1 or 0,
// depending on which is closer representation.
static void writeToTimer(volatile uint8_t *PORT, uint8_t pin, uint8_t value) {
    switch (pinToTimer(PORT, pin)) {
    case TIMER0A:
        // connect pwm to pin on timer 0, channel A
        sbi(TCCR0A, COM0A1);
        OCR0A = value; // set pwm duty
        break;

    case TIMER0B:
        // connect pwm to pin on timer 0, channel B
        sbi(TCCR0A, COM0B1);
        OCR0B = value; // set pwm duty
        break;

    case TIMER1A:
        // connect pwm to pin on timer 1, channel A
        sbi(TCCR1A, COM1A1);
        OCR1A = value; // set pwm duty
        break;

    case TIMER1B:
        // connect pwm to pin on timer 1, channel B
        sbi(TCCR1A, COM1B1);
        OCR1B = value; // set pwm duty
        break;

    case TIMER2A:
        // connect pwm to pin on timer 2, channel A
        sbi(TCCR2A, COM2A1);
        OCR2A = value; // set pwm duty
        break;

    case TIMER2B:
        // connect pwm to pin on timer 2, channel B
        sbi(TCCR2A, COM2B1);
        OCR2B = value; // set pwm duty
        break;

    case NOT_ON_TIMER:
        digitalWrite(PORT, pin, HIGH & (value >> 7)); // 0 if <128
    }
}

// analogRead reads analog signal from pin at PORT C.
uint16_t analogRead(uint8_t pin) {
    // set the analog reference (high two bits of ADMUX) and select the channel (low 4 bits).
    // This also sets ADLAR (left-adjust result) to 0 (the default).
    ADMUX = _BV(REFS0) | (pin & 0b00000111);

    // Start the conversion
    ADCSRA |= _BV(ADSC);

    // ADSC is cleared when the conversion finishes
    while (ADCSRA & _BV(ADSC))
        ;

    // reading result after conversion
    return ADCL | (ADCH << 8);
}

// pulseIn waits for state change of pin at specified PORT.
// pulseIn should be used to monitor time it takes for sent pulse to come back,
// which will be singalled by pin changing state.
// PORT parameter must be a pointer to the according port register defined in avr/io.h.
// PORT must represent port at which desired pin exists.
// If function times out, it returns 0. Otherwise, it returns time it took pulse to return in microseconds.
// It is advised to use only HIGH and LOW as 'value' parameter, otherwise function may show unexpected behaviour.
uint32_t pulseIn(volatile uint8_t *PORT, uint8_t pin, uint8_t state, uint32_t timeout, void (*sendPulse)()) {
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

    // initialize sensore measure process
    sendPulse();

    // wait for the pulse to start
    while ((*PINx(PORT) & bit) != stateMask) {
        if (maxloops-- == 0) {
            return MAX_DISTANCE;
        }
    }

    // wait for the pulse to end
    while ((*PINx(PORT) & bit) == stateMask) {
        if (maxloops-- == 0) {
            return MAX_DISTANCE;
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