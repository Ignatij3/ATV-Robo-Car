#include "libs/distance_sensor/HCSR04.h"
#include "libs/ino_libs/ino_libs.h"
#include "libs/distance_sensor/HCSR04.h"
#include <avr/io.h>
#include "libs/oled/ssd1306.h"
#include <stdio.h>
#include <util/delay.h>

#define MAX__X 127
#define MAX__Y 63
#define MIN__X 0
#define MIN__Y 0

int main(void)
{
  uint8_t addr = SSD1306_ADDR;
  SSD1306_Init (addr);
  SSD1306_ClearScreen ();
  // SSD1306_DrawLine (MIN__X, MAX__X, MIN__Y, MIN__Y); //upwards
  // SSD1306_DrawLine (MAX__X, MAX__X, MIN__Y, MAX__Y); //right
  // SSD1306_DrawLine (MIN__X, MAX__X, MAX__Y, MAX__Y); //bottom
  // SSD1306_DrawLine (MIN__X, MIN__X, MIN__Y, MAX__Y); //left

  // SSD1306_DrawLine (MIN__X, MAX__X, MAX__Y/2, MAX__Y/2); 
  // SSD1306_DrawLine (MIN__X, MAX__X, MAX__Y/4, MAX__Y/4); 
  // SSD1306_DrawLine (MIN__X, MAX__X, MAX__Y*3/4, MAX__Y*3/4); 
  SSD1306_SetPosition (0, 0);
  SSD1306_DrawString ("SELECT MODE");
  SSD1306_SetPosition (7, 2);
  SSD1306_DrawString ("SLAVE");
  SSD1306_SetPosition (7, 4);
  SSD1306_DrawString ("Autonomius");
  SSD1306_SetPosition (7, 6);
  SSD1306_DrawString ("Following");
  // SSD1306_DrawLine (0, MAX_X, 18, 18);
  // SSD1306_DrawString ("2021");
  // SSD1306_UpdateScreen (addr);
  // SSD1306_InvertRectangle(addr, 0, 0, MAX_X, 20);
  SSD1306_UpdateScreen(addr); 
  return 0;
}  