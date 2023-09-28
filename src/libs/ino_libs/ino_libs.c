#include "ino_libs.h"
#include <avr/cpufunc.h>
#include <avr/io.h>
#include <stdint.h>

#define DDx(port) (port - 0x01)
#define PINx(port) (port - 0x02)

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
    uint8_t gate = *DDx(PORT) & _BV(pin);
    // if the pin is input, don't clear bit
    *PORT &= ~_BV(pin) | ~gate;
    // if pin is input, write nothing, else 1 or 0
    *PORT |= ((value & 1) & (gate >> pin)) << pin;
}

void analogWrite(volatile uint8_t *PORT, uint8_t pin, uint8_t value) {}

uint8_t digitalRead(volatile uint8_t *PORT, uint8_t pin) {
    return (*PINx(PORT) >> pin) & 1;
}

uint8_t analogRead(volatile uint8_t *PORT, uint8_t pin) {
    return 0;
}