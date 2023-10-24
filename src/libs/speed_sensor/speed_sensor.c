#include "../ino_libs/ino_libs.h"
#include "../oled/ssd1306.h"
#include "speed_sensor.h"

#define SPEED_SENSOR PIND3

void registerSpeedSensor(void){
    pinMode(&PORTD, SPEED_SENSOR, INPUT_PULLUP);
}

struct timespec {
    time_t tv_sec;
    long tv_nsec;
};

#if defined(_WIN64) // Windows
// #if defined(_WIN32) || defined(_WIN64) // Windows

#include <windows.h>

// Function to get the current time in nanoseconds
long long getNanoTime() {
    LARGE_INTEGER time, frequency;
    QueryPerformanceCounter(&time);
    QueryPerformanceFrequency(&frequency);
    return (long long)(time.QuadPart * 1000000000LL / frequency.QuadPart);
}

#else // shiteaters

#include <time.h>

// Function to get the current time in nanoseconds
long long getNanoTime() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (long long)(ts.tv_sec * 1000000000LL + ts.tv_nsec);
}

#endif

void updateSpeed(void){
    long long startTime, endTime, elapsedTime;
    while (!digitalRead(&PORTD, SPEED_SENSOR)){
    }
    startTime = getNanoTime();
    while (digitalRead(&PORTD, SPEED_SENSOR)){
    }
    endTime = getNanoTime();
    elapsedTime = endTime - startTime;
    //formula 
    //setSpeed_OLED(formula);
    return;
}