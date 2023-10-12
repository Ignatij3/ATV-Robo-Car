#include "../global_constants/global_constants.h"
#include "../oled/ssd1306.h"
#include "../serial_communication/serial_communication.h"
#include "ino_libs.h"
#include <avr/cpufunc.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>

#define DDx(port) (port - 0x01)
#define PINx(port) (port - 0x02)

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define clockCyclesPerMicrosecond() (CPU_CLOCK / 1000000L)
#define clockCyclesToMicroseconds(a) ((a) / clockCyclesPerMicrosecond())
#define microsecondsToClockCycles(a) ((a)*clockCyclesPerMicrosecond())

#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER2A 7
#define TIMER2B 8

static uint8_t pinToTimer(volatile uint8_t *PORT, uint8_t pin);
static void writeToTimer(volatile uint8_t *PORT, uint8_t pin, uint8_t value);

void intToStr(uint8_t num, char *str) {
    sprintf(str, "%d", num);
}

// initPWMTimers initializes timers 0, 1, 2 for phase Correct PWM signal.
void initPWMTimers(void) {
    TCCR0A |= _BV(WGM00) | _BV(COM0A1);
    TCCR1A |= _BV(WGM00) | _BV(COM1A1);
    TCCR2A |= _BV(WGM00) | _BV(COM2A1);
}

// enablePWM turns on waveform generators.
void enablePWM(void) {
    TCCR0B |= _BV(CS01);
    TCCR1B |= _BV(CS11);
    TCCR2B |= _BV(CS21);
}

// disablePWM turns off waveform generators.
void disablePWM(void) {
    TCCR0B &= ~_BV(CS01);
    TCCR1B &= ~_BV(CS11);
    TCCR2B &= ~_BV(CS21);
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

void intToString(uint8_t num, char* str) {
    int i = 0;
    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);
    str[i] = '\0';
    int len = i;
    for (int j = 0; j < len / 2; j++) {
        char temp = str[j];
        str[j] = str[len - j - 1];
        str[len - j - 1] = temp;
    }
}

char* decimalToBinary(int decimal) {
    int i = 0;
    while (decimal > 0) {
        binary[i] = (decimal % 2) + '0'; // Convert the remainder to a character
        decimal /= 2;                    // Move to the next bit
        i++;
    }
    if (i == 0) {
        binary[i] = '0';
        i++;
    }
    binary[i] = '\0';
    int left = 0;
    int right = i - 1;
    while (left < right) {
        char temp = binary[left];
        binary[left] = binary[right];
        binary[right] = temp;
        left++;
        right--;
    }
}

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

// analogRead reads PWM waveform generator duty cycle length from pin at specified PORT.
// PORT parameter must be a pointer to the according port register defined in avr/io.h.
// PORT must represent port at which desired pin exists.
uint8_t analogRead(volatile uint8_t *PORT, uint8_t pin) {
    // set the analog reference (high two bits of ADMUX) and select the
    // channel (low 4 bits).  this also sets ADLAR (left-adjust result)
    // to 0 (the default).
    ADMUX = _BV(6) | (pin & 0x07);

    // start conversion
    sbi(ADCSRA, ADSC);

    // ADSC is cleared when the conversion finishes
    while (bit_is_set(ADCSRA, ADSC))
        ;

    // ADC macro takes care of reading ADC register.
    // avr-gcc implements the proper reading order: ADCL is read first.
    return ADC;
}

// pulseIn waits for state change of pin at specified PORT.
// pulseIn should be used to monitor time it takes for sent pulse to come back,
// which will be singalled by pin changing state.
// PORT parameter must be a pointer to the according port register defined in avr/io.h.
// PORT must represent port at which desired pin exists.
// If function times out, it returns UINT8_MAX. Otherwise, it returns time it took pulse to return in microseconds.
uint32_t pulseIn(volatile uint8_t *PORT, uint8_t pin, uint8_t state, uint32_t timeout, void (*timeTravel)()) {
    // cache the port and bit of the pin in order to speed up the
    // pulse width measuring loop and achieve finer resolution.  calling
    // digitalRead() instead yields much coarser resolution.
    uint8_t bit = _BV(pin);
    // uint8_t stateMask = bit & (state << pin);
    uint8_t stateMask = (state ? bit : 0);
    uint32_t width = 0; // keep initialization out of time critical area

    char portStr[10];
    char bitStr[10];
    char stateStr[10];
    char stateMaskStr[10];

    decimalToBinary(*PORT, portStr);
    decimalToBinary(bit, bitStr);
    decimalToBinary(state, stateStr);
    decimalToBinary(stateMask, stateMaskStr);

    // convert the timeout from microseconds to a number of times through
    // the initial loop; it takes 16 clock cycles per iteration.
    uint32_t maxloops = microsecondsToClockCycles(timeout) / 16;

    decimalToBinary(*PINx(PORT), portStr);
    writeString(" 0pinmask:");
    writeString(portStr);

    // wait for any previous pulse to end

    while ((*PINx(PORT) & bit) == stateMask) {
    }

    decimalToBinary(*PINx(PORT), portStr);
    writeString(" 1pinmask:");
    writeString(portStr);
    timeTravel();
    // wait for the pulse to start
    while ((*PINx(PORT) & bit) != stateMask) {
        if (maxloops-- == 0) {
            return 0;
        }
        width++;
    }
    decimalToBinary(*PINx(PORT), portStr);
    writeString(" 2pinmask:");
    writeString(portStr);

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
