#include "ino_libs.h"
#include <avr/pgmspace.h>

#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER2A 7
#define TIMER2B 8

#define digitalPinToTimer(P) (pgm_read_byte(digital_pin_to_timer_PGM + (P)))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];

static void startConversion(void) {
    sbi(ADCSRA, ADSC);
}

static void writeToTimer(volatile uint8_t *PORT, uint8_t pin, uint8_t value) {
    switch (digitalPinToTimer(pin)) {
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
    default:
        digitalWrite(PORT, pin, HIGH & (value >> 7)); // 0 if <128
    }
}