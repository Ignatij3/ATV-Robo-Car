#include "../oled/ssd1306.h"
#include "central_controller.h"

#define OLED_ADDRESS SSD1306_ADDR // oled display

void _controllerInitOLED(void) {
    SSD1306_Init(OLED_ADDRESS);
}