#include "../distance_sensor/HCSR04.h"
#include "../engine_controller/engine_controller.h"
#include "../global_constants/global_constants.h"
#include "../oled/ssd1306.h"
#include "../speed_sensor/speed_sensor.h"
#include "central_controller.h"
#include <avr/eeprom.h>

// EEPROM address for data storage
#define EEPROM_ADDRESS *((uint16_t *)0x00)

// _controllerInitOLED initializes OLED display
void _controllerInitOLED(void) {
    SSD1306_Init(OLED_ADDRESS);
}

// updateCarMetrics updates information about car on OLED screen
void updateCarMetrics(void) {
    // update speed reading
    setSpeed_OLED(getSensorSpeedReading());

    // update distance reading
    setDistance_OLED(measureDistanceCm());

    // read how many seconds is saved in memory
    uint16_t secondsRecorded = eeprom_read_word(&EEPROM_ADDRESS);

    // write updated seconds to memory
#ifndef DEBUG
    eeprom_write_word(&EEPROM_ADDRESS, ++secondsRecorded);
#else
    eeprom_write_word(&EEPROM_ADDRESS, 0);
#endif // DEBUG

    // update timer
    setTime_OLED(secondsRecorded);

    // update showed direction
    if (isReverse()) {
        setDirection_OLED("Backwards");
    } else {
        uint8_t leftSpeed = getLeftSpeed();
        uint8_t rightSpeed = getRightSpeed();

        if (leftSpeed < rightSpeed) {
            setDirection_OLED("Left turn");
        } else if (leftSpeed > rightSpeed) {
            setDirection_OLED("Right turn");
        } else {
            setDirection_OLED("Forward");
        }
    }
}