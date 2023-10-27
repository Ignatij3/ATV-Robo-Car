#include "../distance_sensor/HCSR04.h"
#include "../global_constants/global_constants.h"
#include "../oled/ssd1306.h"
#include "../speed_sensor/speed_sensor.h"
#include "central_controller.h"
#include <avr/eeprom.h>

// EEPROM address for data storage
#define EEPROM_ADDRESS *((uint8_t *)0x00)

void _controllerInitOLED(void) {
    SSD1306_Init(OLED_ADDRESS);
}

void updateCarSpeed(void) {
    setSpeed_OLED(getSensorSpeedReading());
}

// updateCarTime reads and updates time from persistent memory,
// then outputs to OLED screen.
void updateCarTime(void) {
#ifndef DEBUG
    uint16_t readData;
    // Read how many seconds is saved in memory
    readData = eeprom_read_byte(&EEPROM_ADDRESS);

    // Write updated seconds in memory
    eeprom_write_byte(&EEPROM_ADDRESS, ++readData);

    setTime_OLED(readData); // updates the screen with new time value
#else
    eeprom_write_byte(&EEPROM_ADDRESS, 0);
    setTime_OLED(49999);
#endif // DEBUG
}

void updateCarDistance(void) {
    // setDistance_OLED(measureDistanceCm());
    setDistance_OLED(111);
}