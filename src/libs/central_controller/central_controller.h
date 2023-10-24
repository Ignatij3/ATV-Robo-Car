#ifndef ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H
#define ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    NONE,
    SLAVE,
    CONTROLLED,
    AUTOMATIC
} drivingMode;

typedef enum {
    LEFT,
    CENTERED,
    RIGHT
} linePosition;

// General API
// ------------------------------------------------------------------------------------
void initializeModules(uint8_t minDistance);

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
void _controllerInitJoystick();
drivingMode readNewMode(void);
void setMode(drivingMode drMode);
drivingMode getMode(void);
bool joystickPressed(void);

// OLED screen API
// ------------------------------------------------------------------------------------
void _controllerInitOLED(void);

// IR reflection sensor API
// ------------------------------------------------------------------------------------
void _controllerInitIRSensor(void);
linePosition updateLinePosition(void);
void adjustEnginesSpeed(uint8_t adjustmentFactor);

// Serial communication wrappers
// ------------------------------------------------------------------------------------
void _controllerInitSerial(void);

#endif // ATV_CENTRAL_CONTROLLER_CENTRAL_CONTROLLER_H