#include "ino_libs.h"
#include <avr/cpufunc.h>
#include <avr/pgmspace.h>
#include <stdint.h>

#define F_CPU 1843200 // Clock Speed
#define DDx(port) (port - 0x01)
#define PINx(port) (port - 0x02)

#define clockCyclesPerMicrosecond() (F_CPU / 1000000L)
#define clockCyclesToMicroseconds(a) ((a) / clockCyclesPerMicrosecond())
#define microsecondsToClockCycles(a) ((a)*clockCyclesPerMicrosecond())

void pinMode(volatile uint8_t *PORT, uint8_t pin, uint8_t mode) {
    // mode&1 used in case pullup mode passed, to be left with only first bit
    *DDx(PORT) &= ~_BV(pin);         // clear bit
    *DDx(PORT) |= (mode & 1) << pin; // write either 1 or 0

    // setting pullup setting to PORT address/*
    *PORT &= ~_BV(pin);          // clear bit
    *PORT |= (mode >> 1) << pin; // write either 1 or 0 (pullup bit)
    _NOP();
}

void digitalWrite(volatile uint8_t *PORT, uint8_t pin, uint8_t value) {
    uint8_t mask = *DDx(PORT) & _BV(pin);
    // if the pin is input, don't clear bit
    *PORT &= ~_BV(pin) | ~mask;
    // if pin is input, write nothing, else 1 or 0
    *PORT |= (value << pin) & mask;
}

uint8_t digitalRead(volatile uint8_t *PORT, uint8_t pin) {
    return (*PINx(PORT) >> pin) & 1;
}

void analogWrite(volatile uint8_t *PORT, uint8_t pin, uint8_t value) {
    if (value == 0) {
        digitalWrite(PORT, pin, LOW);
    } else if (value == 255) {
        digitalWrite(PORT, pin, HIGH);
    } else {
        writeToTimer(PORT, pin, value);
    }
}

uint8_t analogRead(volatile uint8_t *PORT, uint8_t pin) {
    // set the analog reference (high two bits of ADMUX) and select the
    // channel (low 4 bits).  this also sets ADLAR (left-adjust result)
    // to 0 (the default).
    ADMUX = _BV(6) | (pin & 0x07);

    startConversion();

    // ADSC is cleared when the conversion finishes
    while (bit_is_set(ADCSRA, ADSC))
        ;

    // ADC macro takes care of reading ADC register.
    // avr-gcc implements the proper reading order: ADCL is read first.
    return ADC;
}

unsigned long pulseIn(volatile uint8_t *PORT, uint8_t pin, uint8_t state, unsigned long timeout) {
    // cache the port and bit of the pin in order to speed up the
    // pulse width measuring loop and achieve finer resolution.  calling
    // digitalRead() instead yields much coarser resolution.
    uint8_t bit = *PORT & _BV(pin);
    uint8_t stateMask = bit & (state << pin);
    unsigned long width = 0; // keep initialization out of time critical area

    // convert the timeout from microseconds to a number of times through
    // the initial loop; it takes 16 clock cycles per iteration.
    unsigned long numloops = 0;
    unsigned long maxloops = microsecondsToClockCycles(timeout) / 16;

    // wait for any previous pulse to end
    while ((*PORT & bit) == stateMask)
        if (numloops++ == maxloops)
            return 0;

    // wait for the pulse to start
    while ((*PORT & bit) != stateMask)
        if (numloops++ == maxloops)
            return 0;

    // wait for the pulse to stop
    while ((*PORT & bit) == stateMask) {
        if (numloops++ == maxloops)
            return 0;
        width++;
    }

    // convert the reading to microseconds. The loop has been determined
    // to be 20 clock cycles long and have about 16 clocks between the edge
    // and the start of the loop. There will be some error introduced by
    // the interrupt handlers.
    return clockCyclesToMicroseconds(width * 21 + 16);
}