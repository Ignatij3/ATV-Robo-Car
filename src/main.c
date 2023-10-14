#include "libs/distance_sensor/HCSR04.h"
#include "libs/ino_libs/ino_libs.h"
#include "../global_constants/global_constants.h"
#include "libs/distance_sensor/HCSR04.h"
#include <avr/io.h>
#include "libs/oled/ssd1306.h"
#include <stdio.h>
#include <util/delay.h>



#define value 14
int main(void)
{
  SSD1306_Init (addr);  
  int mode = mode_Menu();
  _delay_ms(2000);
  mode = set_Mode(mode, 1);
  _delay_ms(2000);
  mode = set_Mode(mode, 1);
  _delay_ms(2000);
  mode = set_Mode(mode, 1);
  _delay_ms(2000);
  mode = set_Mode(mode, -1);
  _delay_ms(2000);
  mode = set_Mode(mode, -1);
  _delay_ms(2000);
  info_Menu();
  
  return 0;
}  