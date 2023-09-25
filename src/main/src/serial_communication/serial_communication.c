#define __AVR_ATmega328P__

#include "serial_communication.h"
#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CHUNK 16
#define SERIAL_USAGE_INDICATOR 4

volatile static uint8_t rx_buffer[RX_BUFFER_SIZE] = {0};
volatile static uint16_t rx_count = 0;
volatile static uint8_t uart_tx_busy = 1;

void serialInit(uint32_t ubrr) {
    UBRR0H = (ubrr & 0x0F00) >> 8;
    UBRR0L = (ubrr & 0x00FF);

    UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << TXCIE0) | (1 << RXCIE0);
}

void writeByte(uint8_t c) {
    enableLED();
    while (uart_tx_busy == 0)
        ;
    uart_tx_busy = 0;
    UDR0 = c;
    disableLED();
}

void writeArray(uint8_t *c, uint16_t len) {
    enableLED();
    for (uint16_t i = 0; i < len; i++) {
        writeByte(c[i]);
    }
    disableLED();
}

uint16_t readCount(void) { return rx_count; }

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

static void enableLED(void) { digitalWrite(SERIAL_USAGE_INDICATOR, 1); }

static void disableLED(void) { digitalWrite(SERIAL_USAGE_INDICATOR, 0); }