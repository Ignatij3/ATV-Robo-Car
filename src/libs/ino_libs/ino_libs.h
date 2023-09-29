#ifndef ATV_INO_LIBS_INO_LIBS_H
#define ATV_INO_LIBS_INO_LIBS_H
#include <stdint.h>

#define LOW 0
#define HIGH 1

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

void pinMode(volatile uint8_t *PORT, uint8_t pin, uint8_t mode);
void digitalWrite(volatile uint8_t *PORT, uint8_t pin, uint8_t value);
void analogWrite(volatile uint8_t *PORT, uint8_t pin, uint8_t value);
uint8_t digitalRead(volatile uint8_t *PORT, uint8_t pin);
uint8_t analogRead(volatile uint8_t *PORT, uint8_t pin);
unsigned long pulseIn(volatile uint8_t *PORT, uint8_t pin, uint8_t state, unsigned long timeout);

#endif