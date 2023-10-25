#include "../oled/ssd1306.h"
#include <avr/eeprom.h>
#include <avr/io.h>

// EEPROM address for data storage
#define EEPROM_ADDRESS *((uint8_t *)0x00)

// updates car's timer every second by one and write this value to EEPROM: memory whose values are kept when the board
// is turned off. every function call updares OLED diplay with new timer's value.
void updateCarTime(void) {
    uint16_t readData;
    // Read how many seconds is saved in memory
    readData = eeprom_read_byte(&EEPROM_ADDRESS);

    // Write updated seconds in memory
    eeprom_write_byte(&EEPROM_ADDRESS, ++readData);

    setTime_OLED(readData); // updates the screen with new time value

    return;
}
