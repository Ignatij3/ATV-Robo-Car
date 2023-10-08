#include "../ino_libs/ino_libs.h"
#include "serial_communication.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>

#define CHUNK 16
#define SERIAL_USAGE_INDICATOR PIND4
#define RX_BUFFER_SIZE 32

volatile static uint8_t rx_buffer[RX_BUFFER_SIZE] = {0};
volatile static uint16_t rx_count = 0;
volatile static uint8_t uart_tx_busy = 1;

static void enableLED(void);
static void disableLED(void);

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

// writeByte writes single byte via serial interface.
void writeByte(uint8_t c) {
    enableLED();
    while (uart_tx_busy == 0)
        ;
    uart_tx_busy = 0;
    UDR0 = c;
    disableLED();
}

// writeByte writes array of bytes with length 'len' via serial interface.
void writeArray(uint8_t *c, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        writeByte(c[i]);
    }
}

// writeString sends C-style string via serial interface.
void writeString(uint8_t *c) {
    uint16_t i = 0;
    do {
        writeByte(c[i]);
        i++;

    } while (c[i] != '\0');
    writeByte(c[i]);
}

// readCount returns amount of bytes left in the buffer.
uint16_t readCount(void) {
    return rx_count;
}

// readByte reads single byte via serial interface.
uint8_t readByte(void) {
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