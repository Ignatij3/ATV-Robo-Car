#ifndef ATV_GLOBAL_CONSTANTS_GLOBAL_CONSTANTS_H
#define ATV_GLOBAL_CONSTANTS_GLOBAL_CONSTANTS_H
#include <stdint.h>

typedef enum {
    NONE,
    SLAVE,
    CONTROLLED,
    AUTOMATIC
} drivingMode;

#define MAX_DISTANCE UINT8_MAX
#define MIN_DISTANCE 0
#define MAX_SPEED UINT8_MAX
#define MIN_SPEED 0
#define CPU_CLOCK 16000000UL // Clock speed
#define clockCyclesPerMicrosecond() (CPU_CLOCK / 1000000L)
#define clockCyclesToMicroseconds(a) ((a) / clockCyclesPerMicrosecond())
#define microsecondsToClockCycles(a) ((a) * clockCyclesPerMicrosecond())

#endif // ATV_GLOBAL_CONSTANTS_GLOBAL_CONSTANTS_H