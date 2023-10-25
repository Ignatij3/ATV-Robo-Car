#ifndef ATV_JOYSTICK_JOYSTICK_H
#define ATV_JOYSTICK_JOYSTICK_H

#include <stdbool.h>
#include <time.h>

bool pressed(void);
void registerJoystick(void);
uint8_t mapValue(uint16_t myvalue);
uint8_t changeMode(void);

#endif // ATV_JOYSTICK_JOYSTICK_H
