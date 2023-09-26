#ifndef ATV_INO_LIBS_INO_LIBS_H
#define ATV_INO_LIBS_INO_LIBS_H

#define HIGH 255
#define LOW 0

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

void pinMode(uint8_t *PORT, uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t *PORT, uint8_t pin, uint8_t value);
void analogWrite(uint8_t *PORT, uint8_t pin, uint8_t value);
uint8_t digitalRead(uint8_t *PORT, uint8_t pin);
uint8_t analogRead(uint8_t *PORT, uint8_t pin);

#endif