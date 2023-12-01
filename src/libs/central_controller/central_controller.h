#ifndef ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#define ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H

#include "../global_constants/global_constants.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    LEFT,
    CENTERED,
    RIGHT
} linePosition;

// General API
// ------------------------------------------------------------------------------------
void initializeModules(uint8_t minDistance);
bool isPoweredOn(void);

// Ultrasonic distance sensor API
// ------------------------------------------------------------------------------------
void _controllerInitDistanceSensor(uint8_t minimalDistance);
bool isCollisionSoon(void);
void evadeCollision(void);

// Engine API
// ------------------------------------------------------------------------------------
void _controllerInitEngines(void);
void enableCar(void);
void disableCar(void);
void accelerate(uint8_t step);
void decelerate(uint8_t step);
void setEngineSpeed(uint8_t speed);
void reverseEngines(void);

// Joystick API and Car's mode control functions
// ------------------------------------------------------------------------------------
void _controllerInitJoystick(void);
drivingMode readNewMode(void);
drivingMode updateMode(void);
drivingMode getMode(void);
bool joystickPressed(void);
bool isChoosingMode(void);

// Communication with bluetooth module
// ------------------------------------------------------------------------------------
void _controllerInitSerial(void);
void readExecuteCommand(void);

// OLED screen API
// ------------------------------------------------------------------------------------
void _controllerInitOLED(void);
void updateCarMetrics(void);
void _setPowerStr(const char *power);

// IR reflection sensor API
// ------------------------------------------------------------------------------------
void _controllerInitIRSensor(void);
linePosition updateLinePosition(void);
void adjustEnginesSpeed(uint8_t adjustmentFactor);

// Speed sensor API
// ------------------------------------------------------------------------------------
void _controllerInitSpeedSensor(void);
uint8_t getCarSpeedSensor(void);

#endif // ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H