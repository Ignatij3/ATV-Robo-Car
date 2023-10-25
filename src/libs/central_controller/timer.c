#include <avr/io.h>
#include <avr/eeprom.h>
#include "../oled/ssd1306.h"

// EEPROM address for data storage
#define EEPROM_ADDRESS *((uint8_t*)0x00)

void updateCarTime(void) {
    uint16_t readData;
    // Read how many seconds is saved in memory
    readData = eeprom_read_byte(&EEPROM_ADDRESS);

    // Write updated seconds in memory
    eeprom_write_byte(&EEPROM_ADDRESS, ++readData);

    setTime_OLED(readData);

    return;
}