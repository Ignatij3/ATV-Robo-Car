#include "../distance_sensor/HCSR04.h"
#include "../engine_controller/engine_controller.h"
#include "../oled/ssd1306.h"
#include "../speed_sensor/speed_sensor.h"
#include "central_controller.h"

// update rate of 30Hz
#define INTERVAL_MS 1000 / 30

// _controllerInitOLED initializes OLED display
void _controllerInitOLED(void) {
    SSD1306_Init(OLED_ADDRESS);
}

// updateCarMetrics updates information about car on OLED screen
void updateCarMetrics(void) {
    static uint32_t lastUpdate;
    if (millis() - lastUpdate > INTERVAL_MS && !isChoosingMode()) {
        // update speed reading
        setSpeed_OLED(getSensorSpeedReading());

        // update distance reading
        setDistance_OLED(measureDistanceCm());

        // update time
        setTime_OLED(_updateInternalTimer());

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
}