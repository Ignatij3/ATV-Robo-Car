#include "../global_constants/global_constants.h"
#include "central_controller.h"
#include <avr/interrupt.h>

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

// micros returns microseconds elapsed from program's startup.
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