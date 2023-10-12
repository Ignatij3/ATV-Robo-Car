#include "libs/central_controller/central_controller.h"
#include <avr/interrupt.h>
#include <util/delay.h>

bool poweredOn = false;

// captures interrupt from switch pin, switches power to the wheels on or off.
ISR(PCINT0_vect) {
    // button's state: 1 - released, 0 - pressed
    volatile static uint8_t state = 1;

    state = (state - 1) & 1;
    // We don't want to toggle power mode on release
    if (!state) {
        poweredOn = !poweredOn;
    }
}

// setUpInterrupts writes to registers that manage interrupts to capture interrupts from built-in switch.
// Afterwards, it enables interrupts.
void setUpInterrupts(void) {
    EICRA |= _BV(ISC01);
    EIMSK |= _BV(INT0);
    PCICR |= _BV(PCIE0);   // port b
    PCMSK0 |= _BV(PCINT7); // switch pin
    sei();                 // enable interrupts
}

int main(void) {
    initializeModules(10);
    setMode(AUTOMATIC);
    setUpInterrupts();
    enableCar();

    while (1) {
        // halt while car is turned off
        while (!poweredOn) {
        }

        setMode(readNewMode());

        switch (getMode()) {
        // in automatic mode, car drives forward until colliding
        // afterwards, it turns around and continues forward
        case AUTOMATIC:
            accelerate(1);
            if (isCollisionSoon()) {
                evadeCollision();
            }
            break;

        // in controlled mode, car receives and executes commands from DualShock PS4 controller
        case CONTROLLED:
            /* code */
            break;

        // in slave mode, car follows black line. If there is predecessor on a line, the car tailgates it
        case SLAVE:
            /* code */
            break;

        // if NONE mode is chosen, the car must halt
        case NONE:
            return 1;
        }
        // delay not to change state too rapidly
        _delay_ms(10);
    }

    disableCar();
    return 0;
}
#include "libs/ino_libs/ino_libs.h"
#include "libs/distance_sensor/HCSR04.h"
#include <avr/interrupt.h>
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
