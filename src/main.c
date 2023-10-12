#include "libs/ino_libs/ino_libs.h"
#include "libs/distance_sensor/HCSR04.h"
#include <avr/io.h>
#include "libs/oled/ssd1306.h"
#include <stdio.h>
#include <util/delay.h>

int main(void)
{
    char str[20];
  uint8_t addr = SSD1306_ADDR;

  // init ssd1306
  SSD1306_Init (addr);

  // clear screen
  SSD1306_ClearScreen ();
  // draw line
  SSD1306_DrawLine (0, MAX_X, 4, 4);
  // set position
  SSD1306_SetPosition (7, 1);
  // draw string
  SSD1306_DrawString ("SSD1306 OLED DRIVER");
  // draw line
  SSD1306_DrawLine (0, MAX_X, 18, 18);
  // set position
  SSD1306_SetPosition (40, 3);
  // draw string
  SSD1306_DrawString ("MATIS23432");
  SSD1306_UpdateScreen (addr);
  // set position
  SSD1306_SetPosition (53, 5);
  // draw string
  registerDistanceSensor();
//   while(1){
//     SSD1306_ClearScreen ();
//     SSD1306_SetPosition (53, 5);
//     for (int i=0; i<10; i++){
//       uint8_t distance = measureDistanceCm();
//       char str[3]; 
//       sprintf(str, "%u", distance); 
//       SSD1306_DrawString (str);
//       SSD1306_UpdateScreen (addr);
//       _delay_us(100000);
//     }
//   }
  // return value

 while(1){
    uint8_t distance = measureDistanceCm();
    delay(1000);
 } 
  return 0;
}


// // captures interrupt from switch pin, switches power to the wheels on or off.
// ISR(PCINT0_vect) {
//     // button's state: 1 - released, 0 - pressed
//     volatile static uint8_t state = 1;

//     state = (state - 1) & 1;
//     // We don't want to toggle power mode on release
//     if (!state) {
//         togglePower();
//     }
// }

// // setUpInterrupts writes to registers that manage interrupts to capture interrupts from built-in switch.
// // Afterwards, it enables interrupts.
// void setUpInterrupts(void) {
//     EICRA |= _BV(ISC01);
//     EIMSK |= _BV(INT0);
//     PCICR |= _BV(PCIE0);   // port b
//     PCMSK0 |= _BV(PCINT7); // switch pin
//     sei();                 // enable interrupts
// }

// int main(void) {
//     initializeModules(10);
//     setMode(AUTOMATIC);
//     setUpInterrupts();

//     while (1) {
//         run();
//     }
//     return 0;
// }