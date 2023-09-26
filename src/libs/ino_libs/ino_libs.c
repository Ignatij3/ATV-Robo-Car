#include "ino_libs.h"
#include <avr/io.h>
#include <stdint.h>

#define DDx(port) (port - 0x01)
#define PINx(port) (port - 0x02)

void pinMode(uint8_t *PORT, uint8_t pin, uint8_t mode) {
    // mode&1 used in case pullup mode passed, to be left with only first bit
    *DDx(PORT) = (*DDx(PORT) & (~_BV(pin))) | ((mode & 1) << pin);
    // *DDx(PORT) &= ~_BV(pin);
    // *DDx(PORT) |= (mode & 1) << pin;

    // setting pullup setting to PORT address
    *PORT = (*PORT & (~_BV(pin))) | ((mode >> 1) << pin);
    // *PORT &= ~_BV(pin);
    // *PORT |= (mode >> 1) << pin;
    __no_operation();
}

void digitalWrite(uint8_t *PORT, uint8_t pin, uint8_t value) {
    uint8_t gate = *DDx(PORT) & _BV(pin);
    *PORT = ((~_BV(pin) | ~gate) & *PORT) | ((value & 1) & gate);
    // *PORT &= ~_BV(pin) | ~gate;
    // *PORT |= (value & 1) & gate;
}

void analogWrite(uint8_t *PORT, uint8_t pin, uint8_t value) {}

uint8_t digitalRead(uint8_t *PORT, uint8_t pin) { return *PINx(PORT) & pin; }

uint8_t analogRead(uint8_t *PORT, uint8_t pin) {}