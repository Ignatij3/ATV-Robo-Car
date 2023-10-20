#include "serial_communication.h"
#include "../ino_libs/ino_libs.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CHUNK 16
#define SERIAL_USAGE_INDICATOR PIND4
#define RX_BUFFER_SIZE 32

// returns variable arguments as a list
#define COPY_VAR_ARGS(arg)                                                                                             \
    ({                                                                                                                 \
        va_list varArgs;                                                                                               \
        va_start(varArgs, arg);                                                                                        \
        varArgs;                                                                                                       \
    })

volatile static uint8_t rx_buffer[RX_BUFFER_SIZE] = {0};
volatile static uint16_t rx_count = 0;
volatile static uint8_t uart_tx_busy = 1;

static void enableLED(void);
static void disableLED(void);
static char *applyFormat(const char *format, va_list formatArgs);
static size_t determineBufferSize(const char *format, va_list formatArgs);

ISR(USART_RX_vect) {
    volatile static uint16_t rx_write_pos = 0;

    rx_buffer[rx_write_pos] = UDR0;
    rx_count++;
    rx_write_pos++;
    if (rx_write_pos >= RX_BUFFER_SIZE) {
        rx_write_pos = 0;
    }
}

ISR(USART_TX_vect) {
    uart_tx_busy = 1;
}

// serialInit sets microchip's registers for serial communication.
void serialInit(uint32_t ubrr) {
    UBRR0H = (ubrr & 0x0F00) >> 8;
    UBRR0L = (ubrr & 0x00FF);

    UCSR0B |= _BV(TXEN0) | _BV(RXEN0) | _BV(TXCIE0) | _BV(RXCIE0);
    sei();
}

// writeBinary prints passed number to serial in its binary representation.
void writeBinary(uint16_t n) {
    static char binaryStr[17]; // 16 bits + 1 for null-termination
    char *p = binaryStr;

    for (int i = 15; i >= 0; i--) {
        if ((n >> i) & 1) {
            *p = '1';
        } else {
            *p = '0';
        }
        p++;
    }
    *p = '\0';
    writeString(binaryStr);
}

// writeUint writes passed number to serial.
void writeUint(uint16_t n) {
    writeStringF("%u", n);
}

// writeByte writes single byte via serial interface.
void writeByte(const char c) {
    enableLED();
    while (uart_tx_busy == 0)
        ;
    uart_tx_busy = 0;
    UDR0 = c;
    disableLED();
}

// writeString sends C-style string via serial interface.
void writeString(const char *str) {
    uint16_t i = 0;
    do {
        writeByte(str[i]);
        i++;

    } while (str[i] != '\0');
    writeByte(str[i]);
}

// writeStringF sends C-style formatted (via sprintf) string via serial interface.
void writeStringF(const char *format, ...) {
    // get format arguments
    va_list formatArgs = COPY_VAR_ARGS(format);
    char *str = applyFormat(format, formatArgs);

    writeString(str);
    free(str);
}

// writeStringLn sends C-style string via serial interface with newline passed to serial.
void writeStringLn(const char *str) {
    writeString(str);
    writeString(NEWLINE);
}

// readCount returns amount of bytes left in the buffer.
uint16_t readCount(void) {
    return rx_count;
}

// readByte reads single byte via serial interface.
char readByte(void) {
    enableLED();
    static uint16_t rx_read_pos = 0;
    uint8_t data = 0;

    data = rx_buffer[rx_read_pos];
    rx_read_pos++;
    rx_count--;
    if (rx_read_pos >= RX_BUFFER_SIZE) {
        rx_read_pos = 0;
    }
    return data;
    disableLED();
}

// enableLED lights up serial usage indicator LED.
static void enableLED(void) {
    digitalWrite(&PORTD, SERIAL_USAGE_INDICATOR, HIGH);
}

// disableLED disables serial usage indicator LED.
static void disableLED(void) {
    digitalWrite(&PORTD, SERIAL_USAGE_INDICATOR, LOW);
}

// applyFormat returns string with format variables inserted into the string.
// It is mandatory to free() returned string.
static char *applyFormat(const char *format, va_list formatArgs) {
    size_t bufsz = determineBufferSize(format, formatArgs);
    char *str = malloc(bufsz + 1);
    vsnprintf(str, bufsz + 1, format, formatArgs);
    return str;
}

// determineBufferSize returns number of characters (with null terminator counted in)
// needed to hold entire resulting string after substituting formatArgs into format.
static size_t determineBufferSize(const char *format, va_list formatArgs) {
    return vsnprintf(NULL, 0, format, formatArgs);
}