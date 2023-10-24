#ifndef JOYSTICK
#define JOYSTICK
#include <stdbool.h>
#include <time.h>

bool pressed(void);
void registerJoystick(void);
uint8_t mapValue(uint16_t myvalue);
uint8_t changeMode();

#endif

