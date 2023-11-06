#ifndef ATV_TIMER_TIMER_H
#define ATV_TIMER_TIMER_H

#include <stdint.h>

uint32_t millis(void);
uint32_t micros(void);
uint32_t _updateInternalTimer(void);

#endif // ATV_TIMER_TIMER_H