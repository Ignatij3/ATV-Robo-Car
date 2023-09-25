#define __AVR_ATmega328P__

#include "serial_communication.h"
#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CHUNK 16
#define SERIAL_USAGE_INDICATOR 4

// void serialInit(uint8_t ubrr) {
//     UBRR0L = ubrr;
//     UBRR0H = (ubrr >> 8); // (ubrr >> 4)?
//
//     UCSR0A &= ~_BV(U2X0);   // disable double transmission speed
//     UCSR0B |= _BV(TXEN0);   // Turn on transmitter
//     UCSR0B |= _BV(RXEN0);   // Turn on receiver
//     UCSR0C = (3 << UCSZ00); // Set for async. operation, no parity, one stop
//                             // bit, 8 data bits
// }

void serialInit(unsigned int ubrr) {
    /*Set baud rate */
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)(ubrr);
    /*Enable receiver and transmitter */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

static void serialOut(unsigned char ch) {
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    UDR0 = ch;
}

static char serialIn(void) {
    while (!(UCSR0A & (1 << RXC0)))
        ;
    /* Get and return received data from buffer */
    return UDR0;
}

void writeSerial(const char *data) {
    enableLED();
    for (int i = 0; data[i] != '\0' && data[i] != '\n'; i++) {
        serialOut(data[i]);
    }
    serialOut('\n');
    disableLED();
}

const char *readSerial() {
    enableLED();
    char *line = NULL, *tmp = NULL;
    size_t size = 0, index = 0;
    int ch = 1;

    while (ch) {
        ch = serialIn();

        if (ch == '\0' || ch == '\n')
            ch = 0;

        if (size <= index) {
            size += CHUNK;
            tmp = realloc(line, size);
            if (!tmp) {
                free(line);
                line = NULL;
                break;
            }
            line = tmp;
        }

        line[index++] = ch;
    }

    tmp = realloc(line, index + 1);
    if (tmp) {
        line = tmp;
    }

    disableLED();
    return line;
}

static void enableLED(void) { digitalWrite(SERIAL_USAGE_INDICATOR, 1); }

static void disableLED(void) { digitalWrite(SERIAL_USAGE_INDICATOR, 0); }