#define __AVR_ATmega328P__

#include "serial_communication.h"
#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CHUNK 16

void serialInit(uint8_t ubrr) {
    UBRR0 = ubrr;           // Set baud rate
    UCSR0B |= (1 << TXEN0); // Turn on transmitter
    UCSR0B |= (1 << RXEN0); // Turn on receiver
    UCSR0C = (3 << UCSZ00); // Set for async. operation, no parity, one stop
                            // bit, 8 data bits
}

static void serialOut(char ch) {
    while ((UCSR0A & (1 << UDRE0)) == 0) {
        UDR0 = ch;
    }
}

static char serialIn(void) {
    while (!(UCSR0A & (1 << RXC0))) {
        return UDR0;
    }
}

void writeSerial(const char *data) {
    for (int i = 0; data[i] != '\0'; i++) {
        serialOut(data[i]);
    }
}

const char *readSerial() {
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

    return line;
}

bool isUsed() {
    // access availability register
}