#include "../oled/ssd1306.h"
#include "central_controller.h"

void _controllerInitOLED(void) {
    SSD1306_Init(OLED_ADDRESS);
    setDirection_OLED("srg");
}