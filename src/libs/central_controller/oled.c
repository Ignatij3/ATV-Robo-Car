#include "../distance_sensor/HCSR04.h"
#include "../engine_controller/engine_controller.h"
#include "../global_constants/global_constants.h"
#include "../oled/ssd1306.h"
#include "../speed_sensor/speed_sensor.h"
#include "central_controller.h"
#include <avr/eeprom.h>

// EEPROM address for data storage
#define EEPROM_ADDRESS *((uint16_t *)0x00)

#define SECOND 1000

// _controllerInitOLED initializes OLED display
void _controllerInitOLED(void) {
    SSD1306_Init(OLED_ADDRESS);
}

// updateCarMetrics updates information about car on OLED screen
void updateCarMetrics(void) {
    static uint16_t lastTimeUpdate = 0;

    // update speed reading
    setSpeed_OLED(getSensorSpeedReading());

    // update distance reading
    setDistance_OLED(measureDistanceCm());

    // update time
    if (millis() - lastTimeUpdate > SECOND) {
        // read how many seconds is saved in memory and write it back incremented by 1
        eeprom_write_word(&EEPROM_ADDRESS, eeprom_read_word(&EEPROM_ADDRESS) + 1);
        setTime_OLED(eeprom_read_word(&EEPROM_ADDRESS));
    }

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