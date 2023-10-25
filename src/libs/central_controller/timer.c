#include <avr/io.h>
#include <avr/eeprom.h>

// EEPROM address for data storage
uint8_t EEMEM eeprom_address = 0x00;

void updateCarTime(void) {
    uint16_t readData;
    // Read how many seconds is saved in memory
    readData = eeprom_read_byte(&eeprom_address);

    // Write updated seconds in memory
    eeprom_write_byte(&eeprom_address, readData++);
}